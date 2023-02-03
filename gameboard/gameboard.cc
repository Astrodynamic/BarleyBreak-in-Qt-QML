#include "gameboard.h"

#include <algorithm>
#include <random>

namespace {
bool isAdjacent(const GameBoard::Position first,
                const GameBoard::Position second) {
  if (first == second) {
    return false;
  }

  const auto calc_distance = [](size_t pos1, size_t pos2) {
    int distance = static_cast<int>(pos1);
    distance -= static_cast<int>(pos2);
    distance = std::abs(distance);
    return distance;
  };

  bool result{false};

  if (first.first == second.first) {
    int distance = calc_distance(first.second, second.second);
    if (distance == 1) {
      result = true;
    }
  } else if (first.second == second.second) {
    int distance = calc_distance(first.first, second.first);
    if (distance == 1) {
      result = true;
    }
  }

  return result;
}
}  // namespace

GameBoard::GameBoard(const size_t dimension, QObject *parent)
    : QAbstractListModel{parent},
      m_dimension{dimension},
      m_board_size{m_dimension * m_dimension} {
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

  const auto row_index{static_cast<size_t>(index.row())};

  if (!isPositionValid(row_index)) {
    return QVariant();
  }

  return QVariant(static_cast<int>(m_raw_board[row_index].value));
}

bool GameBoard::move(int index) {
  if (!isPositionValid(static_cast<size_t>(index))) {
    return false;
  }

  Position positionOfIndex{GetRowCol(index)};

  auto hiddenElementIterator = std::find(m_raw_board.begin(), m_raw_board.end(), m_board_size);

  Q_ASSERT(hiddenElementIterator != m_raw_board.end());
  Position hiddenElementPosition{GetRowCol(std::distance(m_raw_board.begin(), hiddenElementIterator))};

  if (!isAdjacent(positionOfIndex, hiddenElementPosition)) {
    return false;
  }

  std::swap(hiddenElementIterator->value, m_raw_board[index].value);
  emit dataChanged(createIndex(0, 0), createIndex(m_board_size, 0));
  return true;
}

size_t GameBoard::dimension() const { return m_dimension; }

size_t GameBoard::board_size() const { return m_board_size; }

GameBoard::Position GameBoard::GetRowCol(const size_t index) const {
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
