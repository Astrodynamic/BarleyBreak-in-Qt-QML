#include "gameboard.h"

#include <algorithm>
#include <random>

namespace {
bool isAdjacent(const GameBoard::Position f, const GameBoard::Position s) {
    if (f == s) {
        return false;
    }

    const auto calcDistance = [](const size_t pos_1, size_t pos_2) {
        int distance = static_cast<int>(pos_1);
        distance -= static_cast<int>(pos_2);
        distance = std::abs(distance);
        return distance;
    };

    bool result {false};

    if (f.first == s.second) {
        int distance = calcDistance(f.second, s.second);

        if (distance == 1) {
            result = true;
        }
    } else if (f.second == s.second) {
        int distance = calcDistance(f.first, s.first);

        if (distance == 1) {
            result = true;
        }
    }
    return result;
}
}

GameBoard::GameBoard(const size_t dimension, QObject *parent)
    : QAbstractListModel{parent}
    , m_dimension{dimension}
    , m_board_size{m_dimension * m_dimension} {
    m_raw_board.resize(m_board_size);
    std::iota(m_raw_board.begin(), m_raw_board.end(), 1);
    Shuffle();
}

int GameBoard::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);

    return m_raw_board.size();
}

QVariant GameBoard::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    const int row_index {index.row()};

    if (!isPositionValid(row_index)) {
        return QVariant();
    }

    return QVariant::fromValue(m_raw_board[row_index].value);
}

bool GameBoard::move(int index) {
    if (isPositionValid(static_cast<size_t>(index))) {
        return false;
    }
    const Position element_pos{get_row_col(index)};

    auto hidden_element_it = std::find(m_raw_board.begin(), m_raw_board.end(), board_size());

    Q_ASSERT(hidden_element_it != m_raw_board.end());

    Position hidden_element_pos{get_row_col(std::distance(m_raw_board.begin(), hidden_element_it))};

    if (!isAdjacent(element_pos, hidden_element_pos)) {
        return false;
    }

    std::swap(hidden_element_it->value, m_raw_board[index].value);
    emit dataChanged(createIndex(0, 0), createIndex(m_board_size, 0));

    return true;
}

size_t GameBoard::dimension() const {
    return m_dimension;
}

size_t GameBoard::board_size() const {
    return m_board_size;
}

GameBoard::Position GameBoard::get_row_col(const size_t index) const {
    Q_ASSERT(m_dimension > 0);
    size_t row = index / m_dimension;
    size_t col = index % m_dimension;

    return std::make_pair(row, col);
}

void GameBoard::Shuffle() {
    static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 generator(seed);

    std::shuffle(m_raw_board.begin(), --m_raw_board.end(), generator);
}

bool GameBoard::isPositionValid(const size_t position) const {
    return position < m_board_size;
}
