#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <vector>

class Dialogue {
public:
    void AddLine(const std::string& text);
    std::string GetCurrentLine() const;
    bool HasNext() const;
    void Next();

private:
    std::vector<std::string> m_lines;
    size_t m_currentLine;
};

#endif // DIALOGUE_H
