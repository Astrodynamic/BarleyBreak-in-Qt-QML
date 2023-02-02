#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QAbstractListModel>
#include <vector>

class GameBoard : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int dimension READ dimension CONSTANT)
    Q_PROPERTY(int board_size READ board_size CONSTANT)
public:
    using Position = std::pair<size_t, size_t>;

    static constexpr size_t kDefaultPuzzleDimension {4 };
    GameBoard(const size_t dimension = kDefaultPuzzleDimension, QObject * parent = nullptr);

    struct Block {
        size_t value {};

        Block & operator = (const size_t value) {
            this->value = value;
            return *this;
        }

        bool operator == (const size_t other) {
            return other == value;
        }
    };

    virtual int rowCount(const QModelIndex & parent = QModelIndex {}) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE bool move(int index);

    size_t dimension() const;
    size_t board_size() const;

private:
    std::vector<Block> m_raw_board;

    const size_t m_dimension;
    const size_t m_board_size;

    void Shuffle();
    bool isPositionValid(const size_t position) const;

    Position get_row_col(const size_t index) const;
};

#endif // GAMEBOARD_H
