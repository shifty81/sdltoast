#ifndef CRAFTING_H
#define CRAFTING_H

#include <string>
#include <vector>

class Inventory;

struct CraftingIngredient {
    std::string name;
    int quantity;
};

struct CraftingRecipe {
    std::string resultName;
    int resultQuantity;
    std::vector<CraftingIngredient> ingredients;
};

class Crafting {
public:
    Crafting();

    // Check whether all ingredients for a recipe are present in inventory
    bool CanCraft(int recipeIndex, const Inventory* inventory) const;

    // Consume ingredients and add result to inventory; returns true on success
    bool Craft(int recipeIndex, Inventory* inventory);

    // Access recipes
    int GetRecipeCount() const { return static_cast<int>(m_recipes.size()); }
    const CraftingRecipe& GetRecipe(int index) const { return m_recipes[index]; }

private:
    void InitRecipes();
    std::vector<CraftingRecipe> m_recipes;
};

#endif // CRAFTING_H
