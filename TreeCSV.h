#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>

/* TODO
 * 1. read all file's content
 * 2. create class for parse tree */

std::string buildRow(std::vector<std::string>, char);

class TreeCSV
{
        public:
                TreeCSV();
                TreeCSV(char);
                TreeCSV(std::string);
                TreeCSV(std::string, char);

                /* header line operation */
                void pushHeader(std::vector<std::string>);
                void printHeader(void);
                void updateHeader(std::vector<std::string>);
                void removeHeader(void);
                /* end header line operation */

                /* body line operation */
                void pushBody(std::vector<std::string>);
                void insertBody(int, std::vector<std::string>);
                void prependBody(std::vector<std::string>);
                void updateBody(int, std::vector<std::string>);
                std::string popBody(void);
                std::string eraseBody(int);
                std::string shiftBody(void);
                void printBody(void);
                void printBodyAt(int);
                /* end body line operation */

                void printTable(void);

                void headBody(int);
                void headTable(int);

                int getLines(void);

                char getDelimiter(void);
                void setDelimiter(char);

                void readFile(std::string);
                void writeFile(std::string);
        private:
                int lineNum = 0;
                char delimiter = ',';
                std::vector<std::string> header;
                std::vector<std::string> body;
                std::map<std::string, std::vector<std::string>> item;
};

TreeCSV::TreeCSV()
{
        /* do nothing */
}

TreeCSV::TreeCSV(char delim)
{
        this->setDelimiter(delim);
}

TreeCSV::TreeCSV(std::string filePath)
{
        /* do something */
        std::string content;
        std::ifstream srcFile(filePath, std::ios::in);
        std::vector<std::string> headerTokens;

        while (std::getline(srcFile, content)) {
                if (lineNum == 0) {
                        std::stringstream ss(content);
                        std::string tmp;
                        while (std::getline(ss, tmp, delimiter)) {
                                item[tmp] = std::vector<std::string>();
                                headerTokens.push_back(tmp);
                        }
                        header.push_back(content + "\n");
                        lineNum++;
                } else {
                        std::stringstream ss(content);
                        std::string tmp;
                        int i = 0;
                        while (std::getline(ss, tmp, delimiter)) {
                                item[headerTokens.at(i++)].push_back(tmp);
                        }
                        body.push_back(content + "\n");
                        lineNum++;
                }
        }

        /* clean up */
        srcFile.close();
}

TreeCSV::TreeCSV(std::string filePath, char delim)
{
        this->setDelimiter(delim);
        TreeCSV obj(filePath);
        this->header = obj.header;
        this->body = obj.body;
}

void
TreeCSV::pushHeader(std::vector<std::string> headerContent)
{
        if (header.empty()) {
                /*header.push_back(headerContent + "\n");*/
                std::string tmp = buildRow(headerContent, delimiter);
                header.push_back(tmp);
                lineNum++;
        } else {
                std::cout << "Header already exists." << std::endl;
        }
}

void
TreeCSV::updateHeader(std::vector<std::string> newHeader)
{
        if (!header.empty())
                header.pop_back();
        this->pushHeader(newHeader);
}

void
TreeCSV::removeHeader(void)
{
        if (header.empty()) {
                std::cout << "Header is already empty." << std::endl;
        } else {
                header.clear();
                lineNum--;
        }
}

void
TreeCSV::pushBody(std::vector<std::string> bodyContent)
{
        std::string tmp = buildRow(bodyContent, delimiter);
        body.push_back(tmp);
        lineNum++;
        //body.push_back(bodyContent);
}

void
TreeCSV::insertBody(int index, std::vector<std::string> bodyContent)
{
        if (index < 0) {/* change this */
                std::cout << "Cannot insert value at negative index." << std::endl;
        } else if (index > body.size() - 1) { /* change this to throw exception */
                std::cout << "Index out of range." << std::endl;
        } else {
                std::vector<std::string>::iterator it = body.begin();
                std::string tmp = buildRow(bodyContent, delimiter);
                header.push_back(tmp);
                body.insert(it+index, tmp);
                lineNum++;
        }
}

void
TreeCSV::prependBody(std::vector<std::string> bodyContent)
{
        std::vector<std::string>::iterator it = body.begin();
        std::string tmp = buildRow(bodyContent, delimiter);
        body.insert(it, tmp);
        lineNum++;
}

void
TreeCSV::updateBody(int index, std::vector<std::string> bodyContent)
{
        if (index < 0) {/* change this */
                std::cout << "Cannot update value at negative index." << std::endl;
        } else if (index > body.size() - 1) { /* change this to throw exception */
                std::cout << "Index out of range." << std::endl;
        } else {
                std::string tmp = buildRow(bodyContent, delimiter);
                body.at(index) = tmp;
        }
}

std::string
TreeCSV::popBody(void)
{
        std::string tmp;
        tmp = body.at(body.size() - 1);
        body.pop_back();
        lineNum--;
        return tmp;
}

std::string
TreeCSV::eraseBody(int index)
{
        if (index < 0) {/* change this */
                std::cout << "Cannot erase value at negative index." << std::endl;
                return NULL;
        } else if (index > body.size() - 1) { /* change this to throw exception */
                std::cout << "Index out of range." << std::endl;
                return NULL;
        } else {
                std::vector<std::string>::iterator it = body.begin();
                std::string tmp;
                tmp = body.at(index);
                body.erase(it+index);
                lineNum--;
                return tmp;
        }
}

std::string
TreeCSV::shiftBody(void)
{
        if (body.empty()) { /* change to throw exception */
                std::cout << "Body is empty." << std::endl;
                return NULL;
        } else {
                std::vector<std::string>::iterator it = body.begin();
                std::string tmp;
                tmp = body.at(0);
                body.erase(it);
                lineNum--;
                return tmp;
        }
}

void
TreeCSV::printHeader(void)
{
        if (lineNum == 0) {
                std::cout << "File is empty." << std::endl;
        } else if (header.empty()) {
                std::cout << "File doesn't contain header." << std::endl;
        } else {
                /* has header */
                std::cout << header.at(0);
        }
}

void
TreeCSV::printBody(void)
{
        if (body.empty()) {
                std::cout << "File body is empty." << std::endl;
        } else {
                std::vector<std::string>::iterator it;
                for (it = body.begin(); it != body.end(); ++it) {
                        std::cout << *it;
                }
                std::cout << "\n";
        }
}

void
TreeCSV::printBodyAt(int index)
{
        if (body.empty()) {
                std::cout << "File body is empty." << std::endl;
        } else if (index < 0) { /* change this to throw exception */
                std::cout << "Index cannot be negative." << std::endl;
        } else if (index > body.size() - 1) {
                std::cout << "Index out of range." << std::endl;
        } else {
                std::string tmp;
                tmp = body.at(index);
                this->printHeader();
                std::cout << tmp << "\n";
        }
}

void
TreeCSV::printTable(void)
{
        this->printHeader();
        this->printBody();
}

void
TreeCSV::headBody(int limit = 5)
{
        if (body.empty()) {
                std::cout << "File body is empty." << std::endl;
        } else {
                int counter = 1;
                std::vector<std::string>::iterator it;
                for (it = body.begin(); it != body.end() && counter < limit+1; ++it, ++counter) {
                        std::cout << *it;
                }
                std::cout << "\n";
        }
}

void
TreeCSV::headTable(int limit = 5)
{
        if (body.empty()) {
                std::cout << "File is empty." << std::endl;
        } else {
                std::cout << header.at(0);
                int counter = 1;
                std::vector<std::string>::iterator it;
                for (it = body.begin(); it != body.end() && counter < limit+1; ++it, ++counter) {
                        std::cout << *it;
                }
                std::cout << "\n";
        }
}

int
TreeCSV::getLines(void)
{
        return lineNum;
}

char
TreeCSV::getDelimiter(void)
{
        return delimiter;
}

void
TreeCSV::setDelimiter(char delim)
{
        this->delimiter = delim;
}

void
TreeCSV::readFile(std::string path)
{
        TreeCSV obj(path);
        this->header.clear();
        this->body.clear();

        this->header = obj.header;
        this->body = obj.body;
        lineNum = obj.getLines();
}

void
TreeCSV::writeFile(std::string outfile)
{
        std::ofstream fp;
        std::vector<std::string>::iterator it;
        fp.open(outfile, std::ios::out);
        fp << header.at(0);
        for (it = body.begin(); it != body.end(); ++it)
                fp << *it;
        fp.close();
}

std::string
buildRow(std::vector<std::string> content, char delim)
{
        int i;
        int len = content.size();
        std::string result;
        for (i = 0; i < len; i++) {
                result += content.at(i);
                if (i < len - 1)
                        result += std::string(1, delim);
        }
        result += "\n";
        return result;
}
