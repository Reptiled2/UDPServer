#pragma once

class BufferVector {
private:
	std::vector<char> buffer;
	size_t maxSize = 2048;

public:
	BufferVector() = default;
	BufferVector(std::vector<char> &vector) {
		buffer.insert(buffer.end(), vector.begin(), vector.end());
	};

	const std::vector<char> getBuffer() const {
		return buffer;
	};

	void addElement(const std::vector<char>& vector) {
		buffer.insert(buffer.end(), vector.begin(), vector.end());
	}

	void setPeerId(const int element) {
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

	void addElement(const std::string& element) {
		buffer.insert(buffer.end(), element.begin(), element.end());
		buffer.push_back('\0');
	};
};
