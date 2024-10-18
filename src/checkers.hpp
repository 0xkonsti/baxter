#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#define BOARD_SIZE 8
#define BLACK_START 0xaa55aa0000000000
#define WHITE_START 0x000000000055aa55

#define NORTH_EAST 9
#define NORTH_WEST 7
#define SOUTH_EAST -7
#define SOUTH_WEST -9

#define NO_UP 0xff00000000000000
#define NO_DOWN 0x00000000000000ff
#define NO_LEFT 0x8080808080808080
#define NO_RIGHT 0x0101010101010101

enum class PieceColor { White, Black };
enum class PieceType { Men, King };

struct Piece {
    PieceColor color;
    PieceType type;

    Piece(PieceColor color, PieceType type) : color(color), type(type) {
    }

    bool operator!=(Piece const& other) const {
        return color != other.color || type != other.type;
    }

    bool operator==(Piece const& other) const {
        return color == other.color && type == other.type;
    }

    bool is_king() const {
        return type == PieceType::King;
    }

    std::string to_string() const {
        return color == PieceColor::White ? (type == PieceType::Men ? "w" : "W") : (type == PieceType::Men ? "b" : "B");
    }
};

std::ostream& operator<<(std::ostream& os, Piece const& piece);

struct Position {
    size_t row;
    size_t col;

    Position(size_t row, size_t col) : row(row), col(col) {
    }

    size_t index() const {
        return row * BOARD_SIZE + col;
    }

    std::string to_string() const {
        return "(" + std::to_string(row) + "," + std::to_string(col) + ")";
    }
};

struct Path {
    std::vector<Position> path;
    std::vector<std::optional<Path>> paths;

    Path() = default;

    std::string to_string() const;
};

using Bitboard = uint64_t;

class Checkers {
public:
    Checkers();
    Checkers(Checkers const&);
    ~Checkers() = default;

    std::optional<Piece> get(Position const& position) const;
    bool is_empty(Position const& position) const;
    void set(Position const& position, Piece const& piece);
    void remove(Position const& position);
    /*size_t move(Position const& from, std::vector<Position> const& path);*/

    Path get_moves(Position const& position, bool force_jump = false) const;

    /*size_t count(PieceColor color) const;*/
    /*size_t count(PieceColor color, PieceType type) const;*/

    std::string to_string() const;

private:
    Bitboard white;
    Bitboard black;
    Bitboard kings;

    Bitboard mask(Position const& position) const;
    Bitboard mask(size_t index) const;
    bool is_valid(Position const& position) const;
};

std::ostream& operator<<(std::ostream& os, Checkers const& checkers);
