@echo off
call "D:\MicrosoftVisualStudio2026Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cd /d D:\claude_files\z_game
cmake -B build -G "Ninja"
cmake --build build
