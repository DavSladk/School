#include <kry.h>

Kry::Kry(int argc, char** argv) :
    e{false}, d{false}, c{false}, f{false}, o{false}, v{false},
    keyA{3}, keyB{7}, m1{0}, m2{1}, n{1},
    inputText{""}, outputText{""}, inputFileName{""}, outputFileName{""}, lang{"CZ"}
{
    parseArgs(argc, argv);
}

Kry::~Kry()
{

}

bool gramCompare(const Gram &a, const Gram &b)
{
    return a.count < b.count;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Gram>& obj)
{
    for(auto tmp : obj)
    {
        os << tmp.gram << " " << tmp.count << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& obj)
{
    for(auto tmp : obj)
    {
        os << tmp << std::endl;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Keys>& obj)
{
    for(auto tmp : obj)
    {
        os << "a=" << tmp.a << " b=" << tmp.b << std::endl;
    }
    return os;
}

void Kry::Run()
{
    outputArgs();
    loadFromFile();
    transform(inputText.begin(), inputText.end(), inputText.begin(), toupper);
    // if(v) std::cout << inputText << std::endl;
    if(e)
    {
        cipherWithKey();
    }
    else if(d)
    {
        decipherWithKey();
    }
    else if(c)
    {
        getFrequencies();
        guessKeys();
        decipherWithKey();
    }


    if(o)
    {
        std::ofstream file;
        file.open(outputFileName);
        file << outputText;
        file.close();
    }
    else
    {
        std::cout << outputText << std::endl;
    }
}

void Kry::cipherWithKey()
{
    char currentCipher = 0;
    for (int i = 0; i < inputText.size(); i++)
    {
        if(inputText.compare(i, 1, " ")==0)
        {
            outputText.push_back(' ');
            continue;
        }

        currentCipher = (this->keyA * (inputText[i] - POSITION_OF_A_IN_ASCII) + this->keyB) % SIZE_OF_ALPHABET + POSITION_OF_A_IN_ASCII;
        outputText.push_back(currentCipher);
    }
}

void Kry::decipherWithKey()
{
    float currentPlainChar = 0;
    int m = 1;
    int w = SIZE_OF_ALPHABET - this->keyB;

    while((this->keyA * m) % SIZE_OF_ALPHABET != 1)
    {
        m++;
    }

    for (int i = 0; i < inputText.size(); i++)
    {
        if(inputText.compare(i, 1, " ")==0)
        {
            outputText.push_back(' ');
            continue;
        }

        currentPlainChar = (m*((inputText[i]-POSITION_OF_A_IN_ASCII)+w) % SIZE_OF_ALPHABET) + POSITION_OF_A_IN_ASCII;
        outputText.push_back((char)currentPlainChar);
    }
}

void Kry::getFrequencies()
{
    loadGrams();

    std::stringstream ss(inputText);
    std::string word;

    while (ss >> word)
    {
        for(int i = 0; i < word.size(); i++)
        {
            std::vector<Gram>::iterator iter;

            // count grams
            iter = std::find(cipherFreq.begin(),cipherFreq.end(), std::string() + word[i]);
            if(iter == cipherFreq.end())
            {
                cipherFreq.push_back(Gram(std::string() + word[i], 1));
            }
            else
            {
                (iter->count)++;
            }
        }
    }

    std::sort(openFreq.rbegin(), openFreq.rend(), gramCompare);
    std::sort(cipherFreq.rbegin(), cipherFreq.rend(), gramCompare);
    if(v) std::cout << openFreq;
}

void Kry::guessKeys()
{
    int y;
    int x;
    std::vector<Keys> guessedKeys;

    // Make initial guesses
    y = cipherFreq[0].gram[0]-POSITION_OF_A_IN_ASCII;
    x = openFreq[m1].gram[0]-POSITION_OF_A_IN_ASCII;
    for(int a : coprimes)
    {
        for(int b = 0; b < SIZE_OF_ALPHABET; b++)
        {
            if(y == (a*x+b)%SIZE_OF_ALPHABET)
            {
                guessedKeys.push_back(Keys(a,b));
            }
        }
    }

    y = cipherFreq[1].gram[0]-POSITION_OF_A_IN_ASCII;
    x = openFreq[m2].gram[0]-POSITION_OF_A_IN_ASCII;
    int i = 0;
    for(int a : coprimes)
    {
        for(int b = 0; b < SIZE_OF_ALPHABET; b++)
        {
            if(y == (a*x+b)%SIZE_OF_ALPHABET)
            {
                auto iter = std::find(guessedKeys.begin(), guessedKeys.end(), Keys(a,b));
                if(iter != guessedKeys.end()) // If the key is already present
                {
                    this->keyA = a;
                    this->keyB = b;
                    i++;
                }
            }
        }
    }

    if(v) std::cout << i << " keys combos found\n";
    if(i < 1)
    {
        std::cerr << "No keys found!" << std::endl;
        exit(1);
    }
    else
    {
        std::cout << "a=" << keyA << ",b=" << keyB << std::endl;
    }
}


void Kry::parseArgs(int argc, char **argv)
{
    int i = 1;
    while(i < argc)
    {
        if(strcmp(argv[i], "-e")==0)
        {
            this->e = true;
            i++;
        }
        else if(strcmp(argv[i], "-d")==0)
        {
            this->d = true;
            i++;
        }
        else if(strcmp(argv[i], "-c")==0)
        {
            this->c = true;
            i++;
        }
        else if(strcmp(argv[i], "-a")==0)
        {
            this->keyA = std::stoi(argv[i+1]);
            i += 2;
        }
        else if(strcmp(argv[i], "-b")==0)
        {
            this->keyB = std::stoi(argv[i+1]);
            i += 2;
        }
        else if(strcmp(argv[i], "-f")==0)
        {
            this->f = true;
            this->inputFileName = argv[i+1];
            i += 2;
        }
        else if(strcmp(argv[i], "-o")==0)
        {
            this->o = true;
            this->outputFileName = argv[i+1];
            i += 2;
        }
        else if(strcmp(argv[i], "-v")==0)
        {
            this->v = true;
            i++;
        }
        else if(strcmp(argv[i], "-m")==0)
        {
            this->m1 = std::stoi(argv[i+1]);
            this->m2 = std::stoi(argv[i+2]);
            i+=3;
        }
        else if(strcmp(argv[i], "-l")==0)
        {
            this->lang = argv[i+1];
            i+=2;
        }
        else if(strcmp(argv[i], "-n")==0)
        {
            this->n = std::stoi(argv[i+1]);
            i+=2;
        }
        else
        {
            this->inputText = argv[i];
            i++;
        }
    }
}

void Kry::outputArgs()
{
    if(this->v)
    {
        std::cout << "e: " << this->e << std::endl;
        std::cout << "d: " << this->d << std::endl;
        std::cout << "c: " << this->c << std::endl;
        std::cout << "f: " << this->f << std::endl;
        std::cout << "o: " << this->o << std::endl;
        std::cout << "v: " << this->v << std::endl;
        std::cout << "l: " << this->lang << std::endl;
        std::cout << "n: " << this->n << std::endl;
        std::cout << "map1: " << this->m1 << std::endl;
        std::cout << "map2: " << this->m2 << std::endl;
        std::cout << "keyA: " << this->keyA << std::endl;
        std::cout << "keyB: " << this->keyB << std::endl;
        std::cout << "inputText: " << this->inputText << std::endl;
        std::cout << "inputFile: " << this->inputFileName << std::endl;
        std::cout << "outputFile: " << this->outputFileName << std::endl;
    }
}

void Kry::loadFromFile()
{
    if(this->f)
    {
        std::ifstream file;
        file.open(this->inputFileName);
        for(int i = 0; i < n; i++)
        {
            std::getline(file, this->inputText);
        }
        file.close();
    }
}

void Kry::loadGrams()
{
    std::ifstream file;
    std::string gram;
    std::string count;

    file.open(lang+".freq");
    while(file >> gram && file >> count)
    {
        openFreq.push_back(Gram(gram,std::stoi(count)));
    }
    file.close();
}
