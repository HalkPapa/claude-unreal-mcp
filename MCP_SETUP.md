# MCP (Model Context Protocol) Setup for Unreal Engine Project

## Overview

This document explains how to configure this Unreal Engine 5.6 project for use with MCP (Model Context Protocol) integration, allowing Claude Code and other AI tools to work effectively with the project.

## Repository Information

- **Repository URL**: https://github.com/HalkPapa/claude-unreal-mcp
- **Project Type**: Unreal Engine 5.6 Blueprint Project
- **Target Platforms**: Mobile (iOS/Android), Desktop (Mac/Windows/Linux), VR

## MCP Configuration

### 1. Claude Code Integration

This project is already configured for Claude Code with:
- Comprehensive `CLAUDE.md` documentation in English and Japanese
- Proper `.gitignore` for Unreal Engine projects
- Project structure optimized for AI assistance

### 2. GitHub Repository Setup

```bash
# Clone the repository
git clone https://github.com/HalkPapa/claude-unreal-mcp.git
cd claude-unreal-mcp

# Open the project
open ClaudeTest/ClaudeTest.uproject
```

### 3. MCP Client Configuration

For MCP clients, use these repository details:

```json
{
  "mcpServer": {
    "name": "claude-unreal-mcp",
    "type": "github",
    "repository": "HalkPapa/claude-unreal-mcp",
    "branch": "main",
    "projectType": "unreal-engine",
    "engineVersion": "5.6"
  }
}
```

## Project Structure for MCP

```
claude-unreal-mcp/
├── CLAUDE.md                    # AI guidance document (English + Japanese)
├── MCP_SETUP.md                # This setup guide
├── .gitignore                  # Unreal Engine specific ignores
├── ClaudeTest/                 # Main Unreal project
│   ├── ClaudeTest.uproject     # Project definition
│   ├── Config/                 # Engine configuration
│   │   ├── DefaultEngine.ini   # Core engine settings
│   │   ├── DefaultGame.ini     # Game-specific settings
│   │   ├── DefaultInput.ini    # Input mapping
│   │   └── DefaultEditor.ini   # Editor configuration
│   └── Content/                # Game assets (empty in template)
```

## Key Features for AI Integration

### 1. Blueprint-Only Project
- No C++ compilation required
- All logic can be created through visual scripting
- Easier for AI to understand and modify

### 2. Mobile-Optimized Configuration
- Configured for mobile development workflow
- Touch input and virtual joysticks ready
- Performance settings optimized

### 3. Advanced Rendering Features
- Lumen Global Illumination
- Hardware Ray Tracing
- Nanite Virtualized Geometry
- All configurable through .ini files

### 4. VR-Ready Setup
- Multiple VR platform support (Oculus, Valve Index, Mixed Reality)
- Enhanced Input System for modern input handling

## AI Assistant Capabilities

With this setup, AI assistants can:

1. **Project Management**
   - Analyze and modify engine configurations
   - Understand project architecture
   - Help with asset organization

2. **Blueprint Development**
   - Guide Blueprint creation and logic
   - Suggest optimization approaches
   - Help with input mapping

3. **Platform Configuration**
   - Adjust settings for different target platforms
   - Configure rendering pipelines
   - Set up build configurations

4. **Documentation**
   - Maintain project documentation
   - Generate technical specifications
   - Create development guides

## Usage Examples

### For Claude Code

```bash
# Navigate to project
cd claude-unreal-mcp

# Ask Claude to help with project setup
claude "Help me configure this Unreal project for mobile VR development"

# Get assistance with Blueprint logic
claude "Create a player movement system using Enhanced Input"

# Configuration help
claude "Optimize the rendering settings for better mobile performance"
```

### For MCP Integration

The repository provides structured access to:
- Project configuration files
- Asset organization patterns
- Development workflows
- Documentation in multiple languages

## Development Workflow with MCP

1. **Project Analysis**: AI can read configuration files to understand current setup
2. **Code Generation**: Generate Blueprint logic descriptions and C++ if needed
3. **Configuration Management**: Modify .ini files for different deployment scenarios  
4. **Documentation**: Maintain comprehensive project documentation
5. **Problem Solving**: Diagnose issues using project structure and logs

## Security and Best Practices

- All sensitive data excluded via `.gitignore`
- No compiled binaries in repository
- Configuration files safely versioned
- Clear documentation for all settings

## Next Steps

1. Open project in Unreal Engine 5.6
2. Configure MCP client to use this repository
3. Start development with AI assistance
4. Use CLAUDE.md for detailed project guidance

---

For detailed development guidance, see [CLAUDE.md](./CLAUDE.md) (Available in English and Japanese)