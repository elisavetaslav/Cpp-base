#pragma once

#include <algorithm>
#include <ctime>
#include <stack>
#include <string>
#include <vector>
#include <random>
#include <valarray>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

private:
    std::vector<std::vector<int>> table_;
    GameStatus status_ = GameStatus::NOT_STARTED;
    std::time_t time_ = 0;
    std::vector<std::string> answer_;
    size_t not_mines_count_ = 0;

    void Open(std::stack<Cell>& stack, Cell cell);

public:
    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;
};
