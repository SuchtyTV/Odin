//
// Created by Niclas Schwalbe on 30.05.21.
//
#include "Odin.h"

void generateAllPawnMoves(std::vector<std::tuple<int, int, Figure>> &moves, const Board &board) {

  int field{7};

  std::list<std::tuple<int, int, Figure>> pawn_moves;

  switch (board.to_move_) {
    case Color::BLACK: generateAllPawnMovesWithBlack(pawn_moves, board);
      break;
    case Color::WHITE: generateAllPawnMovesWithWhite(pawn_moves, board);
      break;
  }

  for (auto p : pawn_moves) {
    moves.push_back(p);
  }
}

void generateAllPawnMovesWithWhite(std::list<std::tuple<int, int, Figure>> &pawn_moves, const Board &board) {
  int field_num = -1;
  int rank = 0;
  int line = -1;
  for (auto field : board) {
    field_num++;
    line++;

    if (line == 8) {
      rank++;
      line = 0;
    }

    if (field != WPAWN.value()) {
      continue;
    }

    if (hasNoFigure(board, rank + 1, line)) {
      int new_field{field_num + 8};
      if (rank == 6) {
        generatePawnPromotion(pawn_moves, field_num, new_field);
      } else {
        pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
      }
    }

    if (rank == 1 && hasNoFigure(board, rank + 2, line)) {
      pawn_moves.push_back(std::make_tuple(field_num, field_num + 16, EMPTY));
    }

    int left = line - 1;
    int right = line + 1;

    if (0 <= left && left < 8 && hasBlackFigure(board, rank + 1, left)) {
      int new_field{field_num + 7};
      if (rank == 6) {
        generatePawnPromotion(pawn_moves, field_num, new_field);
      } else {
        pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
      }
    }
    if (0 <= right && right < 8 && hasBlackFigure(board, rank + 1, right)) {
      int new_field{field_num + 9};
      if (rank == 6) {
        generatePawnPromotion(pawn_moves, field_num, new_field);
      } else {
        pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
      }
    }

  }
}


void generateAllPawnMovesWithBlack(std::list<std::tuple<int, int, Figure>> &pawn_moves, const Board &board) {
  int field_num = -1;
  int rank = 0;
  int line = -1;
  for (auto field : board) {
    field_num++;
    line++;
    if (line == 8) {
      rank++;
      line = 0;
    }

    if (field != BPAWN.value()) {
      continue;
    }

    if (hasNoFigure(board, rank - 1, line)) {
      int new_field{field_num - 8};
      if (rank == 1) {
        generatePawnPromotion(pawn_moves, field_num, new_field);
      } else {
        pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
      }
    }

    if (rank == 6 && hasNoFigure(board, rank - 2, line)) {
      pawn_moves.push_back(std::make_tuple(field_num, field_num - 16, EMPTY));
    }

    int left = line - 1;
    int right = line + 1;

    if (0 <= left && left < 8 && hasWhiteFigure(board, rank - 1, left)) {
      int new_field{field_num - 9};
      if (rank == 1) {
        generatePawnPromotion(pawn_moves, field_num, new_field);
      } else {
        pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
      }
    }
    if (0 <= right && right < 8 && hasWhiteFigure(board, rank - 1, right)) {
      int new_field{field_num - 7};
      if (rank == 1) {
        generatePawnPromotion(pawn_moves, field_num, new_field);
      } else {
        pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
      }
    }

  }

}


