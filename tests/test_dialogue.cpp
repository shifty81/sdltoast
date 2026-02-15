// Harvest Quest — Dialogue system unit tests

#include "systems/Dialogue.h"
#include <cassert>
#include <iostream>

static int s_passed = 0;
static int s_failed = 0;

#define TEST(name) static void name()
#define RUN_TEST(name) do { \
    std::cout << "  " #name "... "; \
    try { name(); std::cout << "PASS" << std::endl; s_passed++; } \
    catch (...) { std::cout << "FAIL" << std::endl; s_failed++; } \
} while(0)
#define ASSERT_TRUE(expr)  do { if (!(expr)) throw 1; } while(0)
#define ASSERT_FALSE(expr) do { if (expr) throw 1; } while(0)
#define ASSERT_EQ(a, b)    do { if ((a) != (b)) throw 1; } while(0)

// --- Legacy linear API tests ---

TEST(test_linear_empty) {
    Dialogue dlg;
    ASSERT_EQ(dlg.GetCurrentLine(), "");
    ASSERT_FALSE(dlg.HasNext());
}

TEST(test_linear_single_line) {
    Dialogue dlg;
    dlg.AddLine("Hello!");
    ASSERT_EQ(dlg.GetCurrentLine(), "Hello!");
    ASSERT_FALSE(dlg.HasNext());
}

TEST(test_linear_multiple_lines) {
    Dialogue dlg;
    dlg.AddLine("Hello!");
    dlg.AddLine("How are you?");
    dlg.AddLine("Goodbye!");
    ASSERT_EQ(dlg.GetCurrentLine(), "Hello!");
    ASSERT_TRUE(dlg.HasNext());

    dlg.Next();
    ASSERT_EQ(dlg.GetCurrentLine(), "How are you?");
    ASSERT_TRUE(dlg.HasNext());

    dlg.Next();
    ASSERT_EQ(dlg.GetCurrentLine(), "Goodbye!");
    ASSERT_FALSE(dlg.HasNext());
}

TEST(test_linear_next_at_end) {
    Dialogue dlg;
    dlg.AddLine("Only line");
    dlg.Next(); // Should not crash
    ASSERT_EQ(dlg.GetCurrentLine(), "Only line");
}

// --- Dialogue tree API tests ---

TEST(test_tree_not_active_initially) {
    Dialogue dlg;
    ASSERT_FALSE(dlg.IsActive());
    ASSERT_TRUE(dlg.GetCurrentNode() == nullptr);
}

TEST(test_tree_start_empty) {
    Dialogue dlg;
    dlg.Start(); // No nodes added
    ASSERT_FALSE(dlg.IsActive());
}

TEST(test_tree_start_with_nodes) {
    Dialogue dlg;
    DialogueNode node;
    node.speakerLine = "Welcome!";
    node.nextNodeIndex = -1;
    dlg.AddNode(node);
    dlg.Start();
    ASSERT_TRUE(dlg.IsActive());
    ASSERT_TRUE(dlg.GetCurrentNode() != nullptr);
    ASSERT_EQ(dlg.GetCurrentNode()->speakerLine, "Welcome!");
}

TEST(test_tree_linear_advance) {
    Dialogue dlg;
    DialogueNode node0;
    node0.speakerLine = "First";
    node0.nextNodeIndex = 1;
    dlg.AddNode(node0);

    DialogueNode node1;
    node1.speakerLine = "Second";
    node1.nextNodeIndex = -1;
    dlg.AddNode(node1);

    dlg.Start();
    ASSERT_EQ(dlg.GetCurrentNode()->speakerLine, "First");

    dlg.Advance();
    ASSERT_EQ(dlg.GetCurrentNode()->speakerLine, "Second");

    dlg.Advance(); // next is -1, should end
    ASSERT_FALSE(dlg.IsActive());
}

TEST(test_tree_branching_choices) {
    Dialogue dlg;
    // Node 0: greeting with two choices
    DialogueNode node0;
    node0.speakerLine = "What do you want?";
    node0.choices.push_back({"Buy items", 1});
    node0.choices.push_back({"Chat", 2});
    node0.nextNodeIndex = -1;
    dlg.AddNode(node0);

    // Node 1: buy items
    DialogueNode node1;
    node1.speakerLine = "Here are my wares!";
    node1.nextNodeIndex = -1;
    dlg.AddNode(node1);

    // Node 2: chat
    DialogueNode node2;
    node2.speakerLine = "Nice weather today!";
    node2.nextNodeIndex = -1;
    dlg.AddNode(node2);

    dlg.Start();
    ASSERT_EQ(dlg.GetCurrentNode()->speakerLine, "What do you want?");

    dlg.SelectChoice(1); // Choose "Chat"
    ASSERT_TRUE(dlg.IsActive());
    ASSERT_EQ(dlg.GetCurrentNode()->speakerLine, "Nice weather today!");
}

TEST(test_tree_choice_ends_dialogue) {
    Dialogue dlg;
    DialogueNode node0;
    node0.speakerLine = "Goodbye?";
    node0.choices.push_back({"Yes", -1}); // -1 ends dialogue
    node0.nextNodeIndex = -1;
    dlg.AddNode(node0);

    dlg.Start();
    dlg.SelectChoice(0);
    ASSERT_FALSE(dlg.IsActive());
}

TEST(test_tree_invalid_choice_index) {
    Dialogue dlg;
    DialogueNode node0;
    node0.speakerLine = "Hello";
    node0.choices.push_back({"Option A", -1});
    node0.nextNodeIndex = -1;
    dlg.AddNode(node0);

    dlg.Start();
    dlg.SelectChoice(5); // Invalid index
    ASSERT_FALSE(dlg.IsActive());
}

TEST(test_tree_advance_does_not_skip_choices) {
    Dialogue dlg;
    DialogueNode node0;
    node0.speakerLine = "Choose:";
    node0.choices.push_back({"A", -1});
    node0.choices.push_back({"B", -1});
    node0.nextNodeIndex = 0; // would loop, but Advance ignores this when choices exist
    dlg.AddNode(node0);

    dlg.Start();
    dlg.Advance(); // Should NOT advance because node has choices
    ASSERT_TRUE(dlg.IsActive());
    ASSERT_EQ(dlg.GetCurrentNode()->speakerLine, "Choose:");
}

int main() {
    std::cout << "=== Dialogue Tests ===" << std::endl;
    RUN_TEST(test_linear_empty);
    RUN_TEST(test_linear_single_line);
    RUN_TEST(test_linear_multiple_lines);
    RUN_TEST(test_linear_next_at_end);
    RUN_TEST(test_tree_not_active_initially);
    RUN_TEST(test_tree_start_empty);
    RUN_TEST(test_tree_start_with_nodes);
    RUN_TEST(test_tree_linear_advance);
    RUN_TEST(test_tree_branching_choices);
    RUN_TEST(test_tree_choice_ends_dialogue);
    RUN_TEST(test_tree_invalid_choice_index);
    RUN_TEST(test_tree_advance_does_not_skip_choices);

    std::cout << std::endl << s_passed << " passed, " << s_failed << " failed" << std::endl;
    return s_failed > 0 ? 1 : 0;
}
