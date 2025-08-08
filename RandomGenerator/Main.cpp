#include <iostream>
#include <Windows.h>
#include <vector>
#include <time.h>

#define Test 0

void RandomGenerator(const char* filename, int drawCount = 5);

int main(int argc, char* argv[])
{
	const char* filename = "Text/PresentationList.txt";
	int drawCount = 5;

#if !_DEBUG
	if (argc == 1)
	{
		std::cout << "���α׷� ��� ����� �Ʒ��� �������ּ���.\n";
		std::cout << "����1: RandomGenerator.exe �����̸�\n";
		std::cout << "����2: RandomGenerator.exe �����̸� ��÷Ƚ��\n";
		return -1;
	}
	
	if (argc == 2)
	{
		filename = argv[1];
	}
	else if (argc == 3)
	{
		filename = argv[1];
		drawCount = atoi(argv[2]);
	}
#endif

	// ���� �õ� ����.
	srand((int)time(nullptr));

#if Test
	for (int ix = 0; ix < 10000; ++ix)
	{
		RandomGenerator(filename, drawCount);
		Sleep(100);
	}
#else
	RandomGenerator(filename, drawCount);
#endif
}

void RandomGenerator(const char* filename, int drawCount)
{
	FILE* file = nullptr;
	fopen_s(&file, filename, "rt");
	if (file == nullptr)
	{
		__debugbreak();
		int errorCode = GetLastError();
		char debugMessage[100] = {};
		sprintf_s(debugMessage, 100, "Failed to open file. errorCode: %d\n", errorCode);
		//OutputDebugStringA(debugMessage);
		std::cout << debugMessage;
		return;
	}

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	//char* buffer = new char[fileSize] + 1;
	char buffer[2048];
	memset(buffer, 0, sizeof(char) * 2048);

	size_t readSize = fread(buffer, sizeof(char), 2048, file);

	char* context = nullptr;
	char* line = strtok_s(buffer, "\n", &context);
	std::vector<const char*> allNames;

	while (line)
	{
		allNames.emplace_back(line);
		line = strtok_s(nullptr, "\n", &context);
	}

	// �̸��� ���� �� ���.
	for (int ix = 0; ix < (int)allNames.size(); ++ix)
	{
		for (int jx = 0; jx < (int)allNames.size() - 1; ++jx)
		{
			if (strcmp(allNames[jx], allNames[jx + 1]) > 0)
			{
				std::swap(allNames[jx], allNames[jx + 1]);
				continue;
			}
		}
	}

	// ������ ��÷ (drawCount ��).
	std::vector<int> tempVector;
	std::vector<int> reserveVector;
	std::vector<const char*> presentationNames;
	tempVector.reserve(allNames.size());
	reserveVector.reserve(allNames.size());
	presentationNames.reserve(allNames.size());

	// ��ǥ�� �ε��� �ӽ� �迭�� �ֱ�.
	for (int ix = 0; ix < (int)allNames.size(); ++ix)
	{
		tempVector.emplace_back(ix);
	}

	// drawCount��ŭ ������ ��÷ ����.
	for (int count = 0; count < drawCount; ++count)
	{
		// 0-23�� ��ȣ�� ���� �迭�� ����.
		int arrayCount = (int)tempVector.size();
		for (int ix = 0; ix < arrayCount; ++ix)
		{
			// 0-23 �ε����� �ϳ��� �����鼭 ���� ����.
			int diff = ((int)tempVector.size());
			int index = (diff * rand()) / (RAND_MAX + 1);
			reserveVector.emplace_back(tempVector[index]);

			tempVector.erase(tempVector.begin() + index);
		}

		// drawCount�� �� ����.
		tempVector.swap(reserveVector);
	}

	// ������ ��÷�� ������ �̸� �迭 ����.
	for (int ix = 0; ix < (int)tempVector.size(); ++ix)
	{
		presentationNames.emplace_back(allNames[tempVector[ix]]);
	}

	std::cout << "\n";

	for (const char* name : presentationNames)
	{
		std::cout << name << "\n";
	}

	fclose(file);
}