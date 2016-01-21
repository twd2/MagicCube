#include "stdafx.h"
#include "MagicCube.h"

#ifndef USE_GL
int textMode(int argc, char *argv[])
{
	Cube cube;

	char f2[FORMAT2_LENGTH];
	for (int i = 0; i < FORMAT2_LENGTH; ++i)
	{
		cin >> f2[i];
	}

	string data = convertFromFormat2(f2);
	cube.Deserialize(data);
	
	CubeSolver *solver = newSolver(cube);
	solver->Solve();
	
	auto steps = ReduceFilter::Filter(solver->Steps);
	steps = NoXYZFilter::Filter(steps);
	steps = ReduceFilter::Filter(steps);
	
	delete solver;

	if (steps.size() > 0)
	{
		printf("%s", stepsToString(steps, '\n').c_str());
	}
	else
	{
		printf("Nothing to do.\n");
	}
	return 0;
}
#else

void libeventError(int errcode)
{
	fprintf(stderr, "libevent fatal error occurred, error code: %d\n", errcode);
	exit(1);
}

void initLibraries()
{
#ifdef _DEBUG
	event_enable_debug_mode(); // may cause memory leak
#endif

	event_set_fatal_callback(libeventError);

#ifdef _WIN32

#ifdef MEM_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	WSADATA data;
	int err = WSAStartup(0, &data);
	err = WSAStartup(data.wVersion, &data);
	assert(err == 0);
#endif

#ifdef EVTHREAD_USE_WINDOWS_THREADS_IMPLEMENTED
	evthread_use_windows_threads(); // may cause memory leak
#endif

#ifdef EVTHREAD_USE_PTHREADS_IMPLEMENTED
	evthread_use_pthreads();
#endif
}

void test()
{
	CubeClient client;
	// while (true)
	{
		bool connected = false;
		for (int i = 0; i < 3; ++i)
		{
			if (client.Connect(string("127.0.0.1"), 2333))
			{
				connected = true;
				break;
			}
		}
		if (!connected)
		{
			log_normal("cannot connect");
			return;
			// continue;
		}
		
		client.Start();

		bool succ = client.Auth("123456");
		log_normal("auth: %d", succ);

		string s;
		while (getline(cin, s))
		{
			client.SendPackage(s);

			if (!client.IsAlive)
			{
				break;
			}
		}

		client.Close();
	}
}
#endif //USE_GL

int main(int argc, char *argv[])
{
	int retcode = 0;

#ifdef USE_GL
	try
	{
		initLibraries();
		test();

		retcode = graphicMode(argc, argv);
	}
	catch (const string &err)
	{
		log_warn("%s\n", err.c_str());
		retcode = 1;
	}
#else
	try
	{
		retcode = textMode(argc, argv);
	}
	catch (const SolverError &err)
	{
		printError(err);
		retcode = 1;
	}
	catch (const CubeError &err)
	{
		printError(err);
		retcode = 1;
	}
#endif //USE_GL

#ifdef MEM_DEBUG
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif 
#endif

	return retcode;
}

