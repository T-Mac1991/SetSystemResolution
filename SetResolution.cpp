
#include <iostream>
#include <Windows.h>
#pragma comment(lib, "ntdll.lib")
extern "C" NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(
	ULONG DesiredResolution,
	BOOLEAN SetResolution,
	PULONG CurrentResolution);
extern "C" NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(
	OUT PULONG MinimumResolution,
	OUT PULONG MaximumResolution,
	OUT PULONG ActualResolution);


int main(int argc, char *argv[])
{
	HANDLE h = GetCurrentProcess();
	DWORD dw = GetPriorityClass(h);
	SetPriorityClass(h, HIGH_PRIORITY_CLASS);
	dw = GetPriorityClass(h);

	ULONG minRes, maxRes, curRes;
	NTSTATUS nts1 = NtQueryTimerResolution(&minRes, &maxRes, &curRes);
	double max = ((double)maxRes) / 10000;
	double min = ((double)minRes) / 10000;

	ULONG desRes = 0;
	if (argc == 2)
	{
		double val = atof(argv[1]);
		if (val <= min && val >= max)
			desRes = (ULONG)(val * 10000);
	}

	bool cmdMode = (desRes > 0) ? true : false;
	if (!cmdMode)
	{
		printf("Minimum Resolution: %lf ms\n", min);
		printf("Maximum Resolution: %lf ms\n", max);
		printf("Current Resolution: %lf ms\n", ((double)curRes) / 10000);

		while (1)
		{
			printf("if you want to set resolution, type a number[%lf - %lf]:\n",
				max, min);

			double val = 0;
			std::cin >> val;
			if (val <= min && val >= max)
			{
				desRes = (ULONG)(val * 10000);
				break;
			}
			std::cin.clear();
			std::cin.ignore();
			printf("error: invalid number\n\n");
		}
	}

	NTSTATUS nts = NtSetTimerResolution(desRes, TRUE, &curRes);
	printf("Set resolution %s! current resolution: %lf ms\n",
		(nts >= 0) ? "succeed" : "failed", ((double)curRes) / 10000);

	if (!cmdMode)
	{
		printf("press any key to exit...\n");
		std::cin.clear();
		std::cin.ignore();
		getchar();
	}
	return 0;
}
