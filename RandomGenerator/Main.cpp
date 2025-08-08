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
		std::cout << "프로그램 사용 방법은 아래를 참고해주세요.\n";
		std::cout << "사용법1: RandomGenerator.exe 파일이름\n";
		std::cout << "사용법2: RandomGenerator.exe 파일이름 추첨횟수\n";
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

	// 랜덤 시드 적용.
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

	// 이름순 정렬 후 출력.
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

	// 무작위 추첨 (drawCount 번).
	std::vector<int> tempVector;
	std::vector<int> reserveVector;
	std::vector<const char*> presentationNames;
	tempVector.reserve(allNames.size());
	reserveVector.reserve(allNames.size());
	presentationNames.reserve(allNames.size());

	// 발표자 인덱스 임시 배열에 넣기.
	for (int ix = 0; ix < (int)allNames.size(); ++ix)
	{
		tempVector.emplace_back(ix);
	}

	// drawCount만큼 무작위 추첨 시작.
	for (int count = 0; count < drawCount; ++count)
	{
		// 0-23번 번호를 가진 배열로 시작.
		int arrayCount = (int)tempVector.size();
		for (int ix = 0; ix < arrayCount; ++ix)
		{
			// 0-23 인덱스를 하나씩 뽑으면서 랜덤 진행.
			int diff = ((int)tempVector.size());
			int index = (diff * rand()) / (RAND_MAX + 1);
			reserveVector.emplace_back(tempVector[index]);

			tempVector.erase(tempVector.begin() + index);
		}

		// drawCount번 더 진행.
		tempVector.swap(reserveVector);
	}

	// 무작위 추첨한 순서로 이름 배열 생성.
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