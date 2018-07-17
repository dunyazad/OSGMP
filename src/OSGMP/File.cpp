#include "File.h"

File::File()
	: m_pFileStream(nullptr)
{
	m_pFileStream = new fstream();
}

File::~File()
{
	if (m_pFileStream != nullptr) {
		delete m_pFileStream;
	}
}

void File::Create(string fileName, bool isBinary)
{
	if (isBinary) {
		(*m_pFileStream).open(fileName, ios::binary | ios::in | ios::out | ios::trunc);
	}
	else {
		(*m_pFileStream).open(fileName, ios::in | ios::out | ios::trunc);
	}
}

void File::Open(string fileName, bool isBinary)
{
	if (isBinary) {
		(*m_pFileStream).open(fileName, ios::binary | ios::in);
	}
	else {
		(*m_pFileStream).open(fileName, ios::in);
	}
}

void File::Close()
{
	(*m_pFileStream).close();
}

bool File::isOpen()
{
	return (*m_pFileStream).is_open();
}

bool File::GetWord(string& word)
{
	return !((*m_pFileStream) >> word).eof();
}

bool File::GetLine(string& line)
{
	return !(getline((*m_pFileStream), line).eof());
}

void File::Read(char* buffer, int length)
{
	(*m_pFileStream).read(buffer, length);
}

void File::Write(char* buffer, int length)
{
	(*m_pFileStream).write(buffer, length);
}

fstream& File::operator << (bool data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (short data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (unsigned short data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (int data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (unsigned int data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (long data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (unsigned long data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (float data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (double data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (string& data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (const string& data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (char* data)
{
	return (fstream&)((*m_pFileStream) << data);
}

fstream& File::operator << (const char* data)
{
	return (fstream&)((*m_pFileStream) << data);
}

//

fstream& File::operator >> (bool& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (short& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (unsigned short& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (int& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (unsigned int& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (long& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (unsigned long& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (float& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (double& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (string& data)
{
	return (fstream&)((*m_pFileStream) >> data);
}

fstream& File::operator >> (char* data)
{
	return (fstream&)((*m_pFileStream) >> data);
}
