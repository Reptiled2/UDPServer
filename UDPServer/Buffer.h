#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <string>

class BufferVector {
private:
	std::vector<char> buffer;
	size_t maxSize;

public:
	BufferVector() : maxSize(2048) {}
	BufferVector(std::vector<char> &vector) {
		buffer.insert(buffer.end(), vector.begin(), vector.end());
	};

	std::vector<char> getBuffer() {
		return buffer;
	};

	void addElement(const std::vector<char>& vector) {
		buffer.insert(buffer.end(), vector.begin(), vector.end());
	}

	void peerId(const int element) {
		std::string str = std::to_string(element);
		buffer.push_back(str.size());
		buffer.push_back('\0');
		buffer.insert(buffer.end(), str.begin(), str.end());
		buffer.push_back('\0');
	};

	void addElement(const char* element) {
		size_t length = std::strlen(element);
		buffer.insert(buffer.end(), element, element+length);
		buffer.push_back('\0');
	};

	void addElement(std::string element) {
		buffer.insert(buffer.end(), element.begin(), element.end());
		buffer.push_back('\0');
	};
};


#endif