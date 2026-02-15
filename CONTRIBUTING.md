# Contributing to Harvest Quest

Thank you for your interest in contributing to Harvest Quest! This is an [Atlas Forge](https://github.com/shifty81/AtlasForge) game project. This document provides guidelines and instructions for contributing.

## Code of Conduct

Please be respectful and constructive in all interactions. We want this to be a welcoming community for everyone.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues. When creating a bug report, include:

- **Description**: Clear description of the bug
- **Steps to Reproduce**: Detailed steps to reproduce the issue
- **Expected Behavior**: What you expected to happen
- **Actual Behavior**: What actually happened
- **Environment**: OS, compiler version, SDL2 version
- **Screenshots**: If applicable

### Suggesting Features

Feature requests are welcome! Please include:

- **Clear description** of the feature
- **Use case**: Why this feature would be useful
- **Mockups/examples**: If applicable

### Code Contributions

#### Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/YOUR_USERNAME/sdltoast.git`
3. Create a branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test your changes
6. Commit: `git commit -m "Add feature: your feature name"`
7. Push: `git push origin feature/your-feature-name`
8. Create a Pull Request

#### Development Setup

See [DEVELOPMENT.md](DEVELOPMENT.md) for detailed setup instructions.

#### Code Style Guidelines

**C++ Standards:**
- Use C++20 features (per Atlas Forge conventions)
- Follow [Atlas Forge naming conventions](https://github.com/shifty81/AtlasForge/blob/main/docs/ATLAS_NAMING_CONVENTIONS.md)
- Use RAII for resource management
- Prefer smart pointers over raw pointers

**Naming Conventions:**
```cpp
// Classes: PascalCase
class PlayerController { };

// Functions: PascalCase
void UpdatePosition();

// Member variables: camelCase with m_ prefix
int m_healthPoints;

// Constants: UPPER_SNAKE_CASE
static constexpr int MAX_HEALTH = 100;

// Local variables: camelCase
int currentHealth = 50;
```

**File Organization:**
- One class per file
- Header file: `.h`
- Implementation file: `.cpp`
- Include guards in all headers

**Comments:**
```cpp
// Use single-line comments for brief explanations
int health; // Current health points

/**
 * Use multi-line comments for class/function documentation
 * Explain what the function does, parameters, and return value
 */
class Player {
    // ...
};
```

#### Commit Messages

Follow conventional commits format:

```
type(scope): subject

body (optional)

footer (optional)
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting)
- `refactor`: Code refactoring
- `test`: Adding tests
- `chore`: Maintenance tasks

**Examples:**
```
feat(combat): Add sword attack animation
fix(player): Fix collision detection bug
docs(readme): Update build instructions
refactor(engine): Improve rendering performance
```

### Asset Contributions

We welcome sprite artists, musicians, and sound designers!

**Graphics:**
- See [assets/README.md](assets/README.md) for specifications
- Use 16-bit pixel art style
- Ensure assets are original or properly licensed

**Music/Sound:**
- OGG format preferred for music
- WAV format for sound effects
- Include license information

**Submission:**
1. Create asset files following guidelines
2. Place in appropriate `assets/` subdirectory
3. Update `CREDITS.md` with attribution
4. Create pull request

### Documentation Contributions

Improving documentation is always welcome:

- Fix typos and grammar
- Clarify confusing sections
- Add examples and tutorials
- Translate to other languages

## Pull Request Process

1. **Update documentation** if you change functionality
2. **Add tests** for new features (when test framework exists)
3. **Follow code style** guidelines
4. **Keep PRs focused** - one feature/fix per PR
5. **Write clear PR description**:
   - What changes were made
   - Why the changes were necessary
   - How to test the changes

### PR Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Code refactoring

## Testing
How were these changes tested?

## Screenshots (if applicable)
Add screenshots for UI changes

## Checklist
- [ ] Code follows project style guidelines
- [ ] Documentation updated
- [ ] All tests pass
- [ ] No new warnings introduced
```

## Development Workflow

### Branch Naming
- `feature/feature-name` - New features
- `fix/bug-description` - Bug fixes
- `docs/topic` - Documentation
- `refactor/area` - Code refactoring

### Testing
Before submitting:
```bash
# Build the project
mkdir build && cd build
cmake ..
cmake --build .

# Run the game
./HarvestQuest

# Test your changes manually
```

### Code Review

All submissions require review. Reviewers will check:
- Code quality and style
- Functionality
- Performance implications
- Documentation completeness

Be patient and receptive to feedback!

## Areas Needing Help

### High Priority
- [ ] Sprite artwork for player character
- [ ] Enemy AI implementation
- [ ] Combat system mechanics
- [ ] Farming system implementation
- [ ] Background music tracks

### Medium Priority
- [ ] NPC dialogue system
- [ ] Inventory UI
- [ ] Map editor tool
- [ ] Save/load system
- [ ] Sound effects

### Low Priority
- [ ] Additional dungeons
- [ ] More crop varieties
- [ ] Easter eggs and secrets
- [ ] Achievements system

## Getting Help

- **Questions**: Open a GitHub Discussion
- **Chat**: Discord (link coming soon)
- **Documentation**: Check existing docs first

## Recognition

Contributors will be:
- Listed in [CREDITS.md](CREDITS.md)
- Mentioned in release notes
- Part of an awesome community!

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Thank you for contributing to Harvest Quest! 🌾⚔️**

*An Atlas Forge game project — see [AtlasForge](https://github.com/shifty81/AtlasForge) for engine documentation.*
