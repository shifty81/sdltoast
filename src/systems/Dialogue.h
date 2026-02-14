#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <string>
#include <vector>

// A single node in a dialogue tree
struct DialogueChoice {
    std::string text;    // Text displayed for the choice
    int nextNodeIndex;   // Index of the next DialogueNode (-1 = end)
};

struct DialogueNode {
    std::string speakerLine;             // What the NPC says
    std::vector<DialogueChoice> choices; // Player responses (empty = linear advance)
    int nextNodeIndex;                   // For linear dialogue (-1 = end)
};

class Dialogue {
public:
    Dialogue();

    // --- Legacy linear API (kept for backward compatibility) ---
    void AddLine(const std::string& text);
    std::string GetCurrentLine() const;
    bool HasNext() const;
    void Next();

    // --- Dialogue tree API ---
    void AddNode(const DialogueNode& node);
    void Start();
    bool IsActive() const { return m_active; }
    const DialogueNode* GetCurrentNode() const;
    void SelectChoice(int choiceIndex);
    void Advance(); // Advance linear node (no choices)

private:
    // Linear lines (legacy)
    std::vector<std::string> m_lines;
    size_t m_currentLine;

    // Tree-based dialogue
    std::vector<DialogueNode> m_nodes;
    int m_currentNodeIndex;
    bool m_active;
};

#endif // DIALOGUE_H
