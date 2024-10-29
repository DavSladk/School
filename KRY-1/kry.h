#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>

struct Gram
{
    std::string gram;
    int count;

    Gram(std::string a, int b)
    {
        this->gram = a;
        this->count = b;
    }

    bool operator==(const std::string& r) const
    {
        return this->gram == r;
    }
};

struct Keys
{
    int a;
    int b;

    Keys(int a, int b)
    {
        this->a = a;
        this->b = b;
    }

    bool operator==(const Keys& r) const
    {
        return this->a == r.a && this->b == r.b;
    }
};

class Kry
{
    public:
        Kry(int argc, char** argv);
        ~Kry();

        void Run();

    private:
        bool e;
        bool d;
        bool c;
        bool f;
        bool o;
        bool v;
        int keyA;
        int keyB;
        int m1;
        int m2;
        int n;
        std::string lang;
        std::string inputText;
        std::string outputText;
        std::string inputFileName;
        std::string outputFileName;

        void cipherWithKey();
        void decipherWithKey();
        void getFrequencies();
        void guessKeys();

        void parseArgs(int argc, char** argv);
        void outputArgs();

        void loadFromFile();
        void loadGrams();

        std::vector<Gram> openFreq;
        std::vector<Gram> cipherFreq;

        const int POSITION_OF_A_IN_ASCII = 65;
        const int SIZE_OF_ALPHABET = 26;
        const std::vector<int> coprimes{1,3,5,7,11,15,17,19,21,23,25};
};
