#include <iostream>
#include <ranges>
#include "checkers.hpp"

int main() {
    auto checkers = Checkers();

    checkers.set({3, 3}, Piece(PieceColor::Black, PieceType::Men));
    checkers.remove({6, 2});

    std::cout << checkers << std::endl;

    auto moves = checkers.get_moves({2, 2});

    std::cout << moves.to_string() << std::endl;

    return 0;
}
