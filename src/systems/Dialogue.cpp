#include "Dialogue.h"

Dialogue::Dialogue()
    : m_currentLine(0)
    , m_currentNodeIndex(-1)
    , m_active(false)
{
}

// --- Legacy linear API ---

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
    return !m_lines.empty() && m_currentLine < m_lines.size() - 1;
}

void Dialogue::Next() {
    if (HasNext()) {
        m_currentLine++;
    }
}

// --- Dialogue tree API ---

void Dialogue::AddNode(const DialogueNode& node) {
    m_nodes.push_back(node);
}

void Dialogue::Start() {
    if (!m_nodes.empty()) {
        m_currentNodeIndex = 0;
        m_active = true;
    }
}

const DialogueNode* Dialogue::GetCurrentNode() const {
    if (m_currentNodeIndex >= 0 &&
        m_currentNodeIndex < static_cast<int>(m_nodes.size())) {
        return &m_nodes[m_currentNodeIndex];
    }
    return nullptr;
}

void Dialogue::SelectChoice(int choiceIndex) {
    const DialogueNode* node = GetCurrentNode();
    if (!node) { m_active = false; return; }
    if (choiceIndex < 0 || choiceIndex >= static_cast<int>(node->choices.size())) {
        m_active = false;
        return;
    }
    int next = node->choices[choiceIndex].nextNodeIndex;
    if (next < 0 || next >= static_cast<int>(m_nodes.size())) {
        m_active = false;
        m_currentNodeIndex = -1;
    } else {
        m_currentNodeIndex = next;
    }
}

void Dialogue::Advance() {
    const DialogueNode* node = GetCurrentNode();
    if (!node) { m_active = false; return; }
    // Only advance linear nodes (no choices)
    if (!node->choices.empty()) return;
    int next = node->nextNodeIndex;
    if (next < 0 || next >= static_cast<int>(m_nodes.size())) {
        m_active = false;
        m_currentNodeIndex = -1;
    } else {
        m_currentNodeIndex = next;
    }
}
