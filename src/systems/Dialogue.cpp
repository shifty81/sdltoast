#include "Dialogue.h"

void Dialogue::AddLine(const std::string& text) {
    m_lines.push_back(text);
}

std::string Dialogue::GetCurrentLine() const {
    if (m_currentLine < m_lines.size()) {
        return m_lines[m_currentLine];
    }
    return "";
}

bool Dialogue::HasNext() const {
    return m_currentLine < m_lines.size() - 1;
}

void Dialogue::Next() {
    if (HasNext()) {
        m_currentLine++;
    }
}
