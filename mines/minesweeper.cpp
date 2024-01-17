#include "minesweeper.h"

void Minesweeper::Open(std::stack<Cell>& stack, Cell cell) {
    if (answer_[cell.y][cell.x] == '-') {
        int sum = 0;
        for (size_t i = static_cast<size_t>(std::fmax(static_cast<int64_t>(cell.y) - 1, 0));
             i < static_cast<size_t>(std::fmin(table_.size(), static_cast<int64_t>(cell.y) + 2)); ++i) {
            for (size_t j = static_cast<size_t>(std::fmax(static_cast<int64_t>(cell.x) - 1, 0));
                 j < static_cast<size_t>(std::fmin(table_[0].size(), static_cast<int64_t>(cell.x) + 2)); ++j) {
                sum += table_[i][j];
            }
        }
        if (sum != 0) {
            answer_[cell.y][cell.x] = static_cast<char>('0' + sum);
            not_mines_count_ -= 1;
        } else {
            for (size_t i = static_cast<size_t>(std::fmax(static_cast<int64_t>(cell.y) - 1, 0));
                 i < static_cast<size_t>(std::fmin(table_.size(), static_cast<int64_t>(cell.y) + 2)); ++i) {
                for (size_t j = static_cast<size_t>(std::fmax(static_cast<int64_t>(cell.x) - 1, 0));
                     j < static_cast<size_t>(std::fmin(table_[0].size(), static_cast<int64_t>(cell.x) + 2)); ++j) {
                    if ((j != cell.x || i != cell.y) && answer_[i][j] == '-') {
                        stack.push({j, i});
                    }
                }
            }
            answer_[cell.y][cell.x] = '.';
            not_mines_count_ -= 1;
        }
    }
};

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    std::vector<int> table(height * width, 0);
    std::fill_n(table.begin(), mines_count, 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(table.begin(), table.end(), g);
    std::vector<std::vector<int>> ttable(height, std::vector<int>(width, 0));
    for (size_t i = 0; i < height; ++i) {
        std::copy_n(&table[i * width], width, ttable[i].begin());
    }
    table_ = ttable;
    status_ = GameStatus::IN_PROGRESS;
    std::string str(width, '-');
    answer_ = std::vector<std::string>(height, str);
    time_ = std::time(nullptr);
    not_mines_count_ = (height * width) - mines_count;
};
Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    std::vector<std::vector<int>> table(height, std::vector<int>(width, 0));
    for (size_t i = 0; i < cells_with_mines.size(); ++i) {
        table[cells_with_mines[i].y][cells_with_mines[i].x] = 1;
    }
    table_ = table;
    status_ = GameStatus::IN_PROGRESS;
    std::string str(width, '-');
    answer_ = std::vector<std::string>(height, str);
    time_ = std::time(nullptr);
    not_mines_count_ = (height * width) - cells_with_mines.size();
};

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    std::vector<int> table(height * width, 0);
    std::fill_n(table.begin(), mines_count, 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(table.begin(), table.end(), g);
    std::vector<std::vector<int>> ttable(height, std::vector<int>(width, 0));
    for (size_t i = 0; i < height; ++i) {
        std::copy_n(&table[i * width], width, ttable[i].begin());
    }
    table_ = ttable;
    status_ = GameStatus::IN_PROGRESS;
    std::string str(width, '-');
    answer_ = std::vector<std::string>(height, str);
    time_ = std::time(nullptr);
    not_mines_count_ = (height * width) - mines_count;
};
void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    std::vector<std::vector<int>> table(height, std::vector<int>(width, 0));
    for (size_t i = 0; i < cells_with_mines.size(); ++i) {
        table[cells_with_mines[i].y][cells_with_mines[i].x] = 1;
    }
    table_ = table;
    status_ = GameStatus::IN_PROGRESS;
    std::string str(width, '-');
    answer_ = std::vector<std::string>(height, str);
    time_ = std::time(nullptr);
    not_mines_count_ = (height * width) - cells_with_mines.size();
};

void Minesweeper::OpenCell(const Cell& cell) {
    if (status_ != GameStatus::DEFEAT && status_ != GameStatus::VICTORY) {
        if (answer_[cell.y][cell.x] != '?') {
            if (table_[cell.y][cell.x] == 1) {
                for (size_t i = 0; i < table_.size(); ++i) {
                    for (size_t j = 0; j < table_[0].size(); ++j) {
                        if (table_[i][j] == 1) {
                            answer_[i][j] = '*';
                        }
                    }
                }
                not_mines_count_ = 0;
                status_ = GameStatus::DEFEAT;
                time_ = std::time(nullptr) - time_;
            } else {
                std::stack<Cell> stack;
                stack.push(cell);
                while (!stack.empty()) {
                    Cell it = stack.top();
                    stack.pop();
                    Open(stack, it);
                }
                if (not_mines_count_ == 0) {
                    status_ = GameStatus::VICTORY;
                    time_ = std::time(nullptr) - time_;
                }
            }
        }
    }
};
void Minesweeper::MarkCell(const Cell& cell) {
    if (status_ != GameStatus::DEFEAT && status_ != GameStatus::VICTORY) {
        if (answer_[cell.y][cell.x] == '-') {
            answer_[cell.y][cell.x] = '?';
        } else if (answer_[cell.y][cell.x] == '?') {
            answer_[cell.y][cell.x] = '-';
        }
    }
};

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return status_;
};

time_t Minesweeper::GetGameTime() const {
    return time_;
};

Minesweeper::RenderedField Minesweeper::RenderField() const {
    return answer_;
};