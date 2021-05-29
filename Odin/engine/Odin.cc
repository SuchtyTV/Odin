//
// Created by Niclas Schwalbe on 05.04.21.
//
#include <chrono>
#include <optional>
#include <list>
#include <tuple>
#include <algorithm>


#include "Odin.h"
#include "Utility.h"


Odin::Odin() :
	start_node_(new Node(Board(OdinConstants::standardBoardFen), std::nullopt, std::nullopt, nullptr)) {
	computingThread_ = std::thread(&Odin::search, this);
	computingThread_.detach();
}



void Odin::setPosition(const std::string& fen, const std::vector<std::string>& moves) {
	start_node_ = std::make_shared<Node>(Board(fen), std::nullopt, std::nullopt, nullptr);
	start_node_->board_.setPosition(fen, moves);
}

void Odin::search() {
	using namespace std::chrono_literals;

	while (in_chess_) {
		while (!searching_) {
			std::this_thread::sleep_for(500ms);
		}
		computeNext();
		positions_calculated_++;
	}

}

double Odin::evaluatePosition(Board& board) {
	return 0.0;
}

void Odin::computeNext() {
	start_node_->evalNextPosition();
}

void Odin::setUpForCalculations() {

}

Board makeMove(const Board& b, std::tuple<int, int, Figure> t) {
	Board new_b{ b };

	int temp = new_b[std::get<0>(t) / 8][std::get<0>(t) % 8];
	new_b[std::get<0>(t) / 8][std::get<0>(t) % 8] = 0;

	if (std::get<2>(t).value() == 0) {
		new_b[std::get<1>(t) / 8][std::get<1>(t) % 8] = temp;
	}
	else {
		new_b[std::get<1>(t) / 8][std::get<1>(t) % 8] = std::get<2>(t).value();
	}

	return new_b;
}

void generateAllLegalMoves(std::vector<std::tuple<int, int, Figure>>& moves, const Board& board) {

	generateAllLegalPawnMoves(moves, board);
	/*generateAllKnightMoves(moves, board);
	generateAllBishopMoves(moves, board);
	generateAllRookMoves(moves,board);
	generateAllQueenMoves(moves, board);
	generateAllKingMoves(moves, board);*/

}


void generateAllMoves(std::vector<std::tuple<int, int, Figure>>& moves, const Board& board) {

	generateAllPawnMoves(moves, board);
	/*generateAllKnightMoves(moves, board);
	generateAllBishopMoves(moves, board);
	generateAllRookMoves(moves,board);
	generateAllQueenMoves(moves, board);
	generateAllKingMoves(moves, board);*/

}

bool checkIfMoveIsIllegalDueCheck(const Board& b, std::tuple<int, int, Figure> move) {
	Board new_board = makeMove(b, move);
	return isInCheck(new_board);
}

bool isInCheck(const Board& b) {
	std::vector<std::tuple<int, int, Figure>> moves;
	generateAllMoves(moves, b);

	int field_num{0};

	auto figure = b.to_move_ == Color::WHITE ? BKING : WKING;

	for (auto p : b) {
		if (p == figure.value()) {
			break;
		}
		field_num++;
	}

	for (auto [from, to, f] : moves) {
		if (to == field_num) {
			return true;
		}
	}


	return false;

}

void generateAllPawnMoves(std::list<std::tuple<int, int, Figure>>& pawn_moves, const Board& board) {

	int field{ 7 };

	switch (board.to_move_) {
	case Color::BLACK:
		generateAllPawnMovesWithBlack(pawn_moves, board);
		break;
	case Color::WHITE:
		generateAllPawnMovesWithWhite(pawn_moves, board);
		break;
	}
}

void generateAllPawnMoves(std::vector<std::tuple<int, int, Figure>>& moves, const Board& board) {

	int field{ 7 };

	std::list<std::tuple<int, int, Figure>> pawn_moves;

	switch (board.to_move_) {
	case Color::BLACK:
		generateAllPawnMovesWithBlack(pawn_moves, board);
		break;
	case Color::WHITE:
		generateAllPawnMovesWithWhite(pawn_moves, board);
		break;
	}

	for (auto p : pawn_moves) {
		moves.push_back(p);
	}
}

void generateAllLegalPawnMoves(std::vector<std::tuple<int, int, Figure>>& moves, const Board& board) {

	std::list<std::tuple<int, int, Figure>> pawn_moves;
	generateAllPawnMoves(pawn_moves, board);
	pawn_moves.erase(std::remove_if(pawn_moves.begin(), pawn_moves.end(), checkIfMoveIsIllegalDueCheck(board)), pawn_moves.end());

	for (auto t : pawn_moves) {
		moves.push_back(t);
	}

}

void generateAllPawnMovesWithWhite(std::list<std::tuple<int, int, Figure>>& pawn_moves, const Board& board) {
	int field = -1;
	for (int rank{ 1 }; rank < 7; rank++) {
		for (int line{ 0 }; line < 8; line++) {

			field++;

			if (board[rank][line] != WPAWN.value()) {
				continue;
			}

			if (hasNoFigure(board, rank + 1, line)) {
				int new_field{ field + 8 };
				if (rank == 6) {
					generatePawnPromotion(pawn_moves, field, new_field);
				}
				else {
					pawn_moves.push_back(std::make_tuple(field, new_field, EMPTY));
				}
			}

			if (rank == 1 && hasNoFigure(board, rank + 2, line)) {
				pawn_moves.push_back(std::make_tuple(field, field + 16, EMPTY));
			}

			int left = line - 1;
			int right = line + 1;

			if (0 <= left && left < 8 && hasBlackFigure(board, rank + 1, left)) {
				int new_field{ field + 7 };
				if (rank == 6) {
					generatePawnPromotion(pawn_moves, field, new_field);
				}
				else {
					pawn_moves.push_back(std::make_tuple(field, new_field, EMPTY));
				}
			}
			if (0 <= right && right < 8 && hasBlackFigure(board, rank + 1, right)) {
				int new_field{ field + 9 };
				if (rank == 6) {
					generatePawnPromotion(pawn_moves, field, new_field);
				}
				else {
					pawn_moves.push_back(std::make_tuple(field, new_field, EMPTY));
				}
			}
		}
	}
}

void generateAllPawnMovesWithBlack(std::list<std::tuple<int, int, Figure>>& pawn_moves, const Board& board) {
	int field_num = -1;
	int rank = 0;
	int line = 0;
	for (auto field : board) {

		field_num++;

		if (field != BPAWN.value()) {
			continue;
		}

		if (hasNoFigure(board, rank - 1, line)) {
			int new_field{ field_num - 8 };
			if (rank == 6) {
				generatePawnPromotion(pawn_moves, field_num, new_field);
			}
			else {
				pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
			}
		}

		if (rank == 1 && hasNoFigure(board, rank - 2, line)) {
			pawn_moves.push_back(std::make_tuple(field_num, field_num + 16, EMPTY));
		}

		int left = line - 1;
		int right = line + 1;

		if (0 <= left && left < 8 && hasWhiteFigure(board, rank + 1, left)) {
			int new_field{ field_num - 7 };
			if (rank == 6) {

			}
			else {
				pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
			}
		}
		if (0 <= right && right < 8 && hasWhiteFigure(board, rank + 1, right)) {
			int new_field{ field_num - 9 };
			if (rank == 6) {
				generatePawnPromotion(pawn_moves, field_num, new_field);
			}
			else {
				pawn_moves.push_back(std::make_tuple(field_num, new_field, EMPTY));
			}
		}
		line++;
		if (line == 8) {
			rank++;
			line = 0;
		}
	}

}



