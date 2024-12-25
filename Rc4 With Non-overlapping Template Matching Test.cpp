#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

void InitializeSAndT(vector<unsigned char>& vS, vector<unsigned char>& vT, const vector<unsigned char>& vkey)
{
	for (int i = 0; i < 256; i++)
	{
		vS[i] = i;
		vT[i] = vkey[i % vkey.size()];
	}
}

void KeyScheduling(vector<unsigned char>& vS, const vector<unsigned char>& vT)
{
	unsigned char Temp = 0;
	for (int i = 0; i < 256; i++)
	{
		Temp = (Temp + vS[i] + vT[i]) % 256;
		swap(vS[i], vS[Temp]);
	}
}

unsigned char GenerateByte(vector<unsigned char>& vS, int& i, int& j)
{
	i = (i + 1) % 256;
	j = (j + vS[i]) % 256;
	swap(vS[i], vS[j]);
	unsigned char T = (vS[i] + vS[j]) % 256;
	return vS[T];
}

void GenerateBytes(vector<unsigned char>& vS, int& i, int& j, vector<unsigned char>& vBytes, int NumberOfBytes)
{
	for (int k = 0; k < NumberOfBytes; k++)
	{
		vBytes.push_back(GenerateByte(vS, i, j));
	}
}

vector<int> DecimalToBinary(int Num)
{
	vector<int> vBinary(8, 0);
	int index = 7;
	while (Num > 0)
	{
		vBinary[index--] = Num % 2;
		Num /= 2;
	}
	return vBinary;
}

void PrintBinary(vector<int>& vBinary)
{
	for (int Bit : vBinary)
	{
		cout << Bit;
	}
}

void PrintBytes(vector<unsigned char>& vBytes)
{
	cout << "Generated Bytes: " << "\n";

	for (int i = 0; i < vBytes.size(); i++)
	{
		cout << "Byte " << i + 1 << ": " << (int)vBytes[i] << endl;
	}

	cout << "\n";
}

void PrintBytesAsBinary(vector<unsigned char>& vBytes)
{
	for (int i = 0; i < vBytes.size(); i++)
	{
		cout << "Byte " << i + 1 << " (Binary): ";
		vector<int> vBinary = DecimalToBinary(vBytes[i]);
		PrintBinary(vBinary);
		cout << "\n";
	}
}

int FindPatternOccurrences(vector<unsigned char>& vBytes, vector<int>& vPattern)
{
	vector<int> bitStream;
	for (unsigned char Byte : vBytes)
	{
		vector<int> vBits = DecimalToBinary(Byte);
		bitStream.insert(bitStream.end(), vBits.begin(), vBits.end());
	}

	int count = 0;
	for (int i = 0; i <= bitStream.size() - vPattern.size(); i++)
	{
		if (equal(vPattern.begin(), vPattern.end(), bitStream.begin() + i))
		{
			count++;
		}
	}

	return count;
}

void PrintPatternOccurrencesCount(vector<int>& vPattern, int Occurrences)
{
	cout << "\nThe pattern { ";
	for (int Bit : vPattern)
	{
		cout << Bit << " ";
	}
	cout << "} Appears " << Occurrences << " times in the bit stream.\n";
}

bool CheckPattern(int Occurrences, int ExpectedOccurrences)
{
	return Occurrences <= ExpectedOccurrences;
}

void PrintCheckPatternResult(int Occurrences, int ExpectedOccurrences)
{
	bool result = CheckPattern(Occurrences, ExpectedOccurrences);

	cout << "Result: " << (result ? "Random" : "Non-random") << "\n";
}

double CalculateChiSquare(double Occurrences, double ExpectedCount)
{
	return ((Occurrences - ExpectedCount) * (Occurrences - ExpectedCount)) / ExpectedCount;
}

bool IsRandomBasedOnChiSquare(double Occurrences, double ExpectedCount)
{
	double chiSquareValue = CalculateChiSquare(Occurrences, ExpectedCount);

	return chiSquareValue <= 5.991; // Static Value means 5%
}

void PrintChiSquareTest(double Occurrences, double ExpectedCount)
{
	if (IsRandomBasedOnChiSquare(Occurrences, ExpectedCount))
	{
		cout << "The result is likely random based on the Chi-Square test." << endl;
	}
	else {
		cout << "The result may not be random based on the Chi-Square test." << endl;
	}
}

double CalculatePValue(double ChiSquareValue, int DegreesOfFreedom = 1)
{
	return 1.0 - exp(-ChiSquareValue / 2.0);  //ليست دقيقة تماما لكن كافة للتوضيح
}

bool IsRandomBasedOnChiSquareAndPValue(double Occurrences, double ExpectedCount)
{
	double ChiSquareValue = CalculateChiSquare(Occurrences, ExpectedCount);
	double PValue = CalculatePValue(ChiSquareValue);

	return PValue > 0.05;
}

void PrintChiSquareTestAndPValueResult(double Occurrences, double ExpectedCount)
{
	if (IsRandomBasedOnChiSquareAndPValue(Occurrences, ExpectedCount))
	{
		cout << "The result is likely random based on the Chi-Square test and P-Value." << endl;
	}
	else
	{
		cout << "The result may not be random based on the Chi-Square test and P-Value." << endl;
	}
}

int main()
{
	vector<unsigned char> vkey = { 1, 2, 3, 4, 5 };
	vector<unsigned char> vS(256), vT(256);

	InitializeSAndT(vS, vT, vkey);

	KeyScheduling(vS, vT);

	int i = 0, j = 0;

	vector<unsigned char> vBytes;

	GenerateBytes(vS, i, j, vBytes, 10);

	PrintBytes(vBytes);

	PrintBytesAsBinary(vBytes);

	vector<int> vPattern1 = { 1,0,1 }, vPattern2 = { 1,0 }, vPattern3 = { 1,1,1,1,0 };

	int Occurrences1 = FindPatternOccurrences(vBytes, vPattern1);

	int Occurrences2 = FindPatternOccurrences(vBytes, vPattern2);

	int Occurrences3 = FindPatternOccurrences(vBytes, vPattern3);

	PrintPatternOccurrencesCount(vPattern1, Occurrences1);

	PrintPatternOccurrencesCount(vPattern2, Occurrences2);

	PrintPatternOccurrencesCount(vPattern3, Occurrences3);

	int ExpectedOccurrences = 4; // نفرض 4 بشكل عشوائي

	cout << "\nExpected Occurrences: {" << ExpectedOccurrences << "}\n\n";

	cout << "{" << Occurrences1 << "}" << " = " << "{" << ExpectedOccurrences << "}\n";
	PrintCheckPatternResult(Occurrences1, ExpectedOccurrences);

	cout << "\n{" << Occurrences2 << "}" << " > " << "{" << ExpectedOccurrences << "}\n";
	PrintCheckPatternResult(Occurrences2, ExpectedOccurrences);

	cout << "\n{" << Occurrences3 << "}" << " < " << "{" << ExpectedOccurrences << "}\n";
	PrintCheckPatternResult(Occurrences3, ExpectedOccurrences);


	cout << "\n\nChiSquar 1: {" << CalculateChiSquare(Occurrences1, ExpectedOccurrences) << "}\n";
	PrintChiSquareTest(Occurrences1, ExpectedOccurrences);

	cout << "\nChiSquar 2: {" << CalculateChiSquare(Occurrences2, ExpectedOccurrences) << "}\n";
	PrintChiSquareTest(Occurrences2, ExpectedOccurrences);

	cout << "\nChiSquar 3: {" << CalculateChiSquare(Occurrences3, ExpectedOccurrences) << "}\n";
	PrintChiSquareTest(Occurrences3, ExpectedOccurrences);



	cout << "\n\nP1-Value: {" << CalculatePValue(CalculateChiSquare(Occurrences1, ExpectedOccurrences)) << "}\n";
	PrintChiSquareTestAndPValueResult(Occurrences1, ExpectedOccurrences);

	cout << "\nP2-Value: {" << CalculatePValue(CalculateChiSquare(Occurrences2, ExpectedOccurrences)) << "}\n";
	PrintChiSquareTestAndPValueResult(Occurrences2, ExpectedOccurrences);

	cout << "\nP3-Value: {" << CalculatePValue(CalculateChiSquare(Occurrences3, ExpectedOccurrences)) << "}\n";
	PrintChiSquareTestAndPValueResult(Occurrences3, ExpectedOccurrences);

	system("pause>0");
}
