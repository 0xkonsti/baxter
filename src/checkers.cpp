#include "checkers.hpp"
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, Piece const& piece) {
    return os << piece.to_string();
}

std::string Path::to_string() const {
    std::string result;
    for (size_t i = 0; i < path.size(); ++i) {
        result += path[i].to_string();
        result += paths[i] ? " -> " + paths[i]->to_string() : "\n";
    }
    return result;
}

Checkers::Checkers() : white(0), black(0), kings(0) {
    white = WHITE_START;
    black = BLACK_START;
    kings = 0;
}

Checkers::Checkers(Checkers const& other) : white(other.white), black(other.black), kings(other.kings) {
}

std::optional<Piece> Checkers::get(Position const& position) const {
    Bitboard mask = this->mask(position);
    if (white & mask) {
        return Piece(PieceColor::White, kings & mask ? PieceType::King : PieceType::Men);
    }
    if (black & mask) {
        return Piece(PieceColor::Black, kings & mask ? PieceType::King : PieceType::Men);
    }
    return {};
}

bool Checkers::is_empty(Position const& position) const {
    Bitboard mask = this->mask(position);
    return !(white & mask) && !(black & mask);
}

void Checkers::set(Position const& position, Piece const& piece) {
    Bitboard mask = this->mask(position);
    if (piece.color == PieceColor::White) {
        white |= mask;
        black &= ~mask;
    } else {
        black |= mask;
        white &= ~mask;
    }
    if (piece.is_king()) {
        kings |= mask;
    } else {
        kings &= ~mask;
    }
}

void Checkers::remove(Position const& position) {
    Bitboard mask = this->mask(position);
    white &= ~mask;
    black &= ~mask;
    kings &= ~mask;
}

Path Checkers::get_moves(Position const& position, bool force_jump) const {
    auto moves = Path();
    auto piece = get(position);
    if (!piece) {
        return moves;
    }

    // TODO: Implement consecutive jumps with recursion
    //  - do recursion on clone of the board with the piece moved and the jumped piece removed
    //      > may not be the most efficient way to do this but its straightforward

    if (piece->is_king()) {
        // TODO: Implement king moves
    } else {
        int dr = piece->color == PieceColor::White ? 1 : -1;
        for (int dc = -1; dc <= 1; dc += 2) {
            if (!is_valid({position.row + dr, position.col + dc})) {
                continue;
            }
            Position next(position.row + dr, position.col + dc);
            if (is_empty(next) && !force_jump) {
                moves.path.push_back(next);
                moves.paths.push_back({});
            } else {
                Position jump(position.row + 2 * dr, position.col + 2 * dc);
                if (is_empty(jump)) {
                    moves.path.push_back(jump);
                    auto clone = Checkers(*this);
                    clone.remove(next);
                    clone.remove(position);
                    clone.set(jump, *piece);
                    auto path = clone.get_moves(jump, true);
                    if (path.path.empty()) {
                        moves.paths.push_back({});
                    } else {
                        moves.paths.push_back({path});
                    }
                }
            }
        }
    }

    return moves;
}

Bitboard Checkers::mask(Position const& position) const {
    return mask(position.index());
}

Bitboard Checkers::mask(size_t index) const {
    return 1ull << index;
}

bool Checkers::is_valid(Position const& position) const {
    return position.row >= 0 && position.row < BOARD_SIZE && position.col >= 0 && position.col < BOARD_SIZE;
}

std::string Checkers::to_string() const {
    std::string result;
    for (size_t row = BOARD_SIZE; row > 0; --row) {
        for (size_t col = 0; col < BOARD_SIZE; ++col) {
            Position position(row - 1, col);
            auto piece = get(position);
            if (piece) {
                result += piece->to_string();
            } else {
                result += ".";
            }
        }
        result += "\n";
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, Checkers const& checkers) {
    return os << checkers.to_string();
}
