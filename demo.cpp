#include "demo.h"

void ChessApp::initializeBoard() {
	// Place pawns
	for (int i = 0; i < 8; ++i) {
		board[i][1] = new ChessPiece{ PAWN, WHITE, i, 1 };
		board[i][6] = new ChessPiece{ PAWN, BLACK, i, 6 };
	}
	// Place rooks
	board[0][0] = new ChessPiece{ ROOK, WHITE, 0, 0 };
	board[7][0] = new ChessPiece{ ROOK, WHITE, 7, 0 };
	board[0][7] = new ChessPiece{ ROOK, BLACK, 0, 7 };
	board[7][7] = new ChessPiece{ ROOK, BLACK, 7, 7 };

	// Place knights
	board[1][0] = new ChessPiece{ KNIGHT, WHITE, 1, 0 };
	board[6][0] = new ChessPiece{ KNIGHT, WHITE, 6, 0 };
	board[1][7] = new ChessPiece{ KNIGHT, BLACK, 1, 7 };
	board[6][7] = new ChessPiece{ KNIGHT, BLACK, 6, 7 };

	// Place bishops
	board[2][0] = new ChessPiece{ BISHOP, WHITE, 2, 0 };
	board[5][0] = new ChessPiece{ BISHOP, WHITE, 5, 0 };
	board[2][7] = new ChessPiece{ BISHOP, BLACK, 2, 7 };
	board[5][7] = new ChessPiece{ BISHOP, BLACK, 5, 7 };

	// Place queens
	board[3][0] = new ChessPiece{ QUEEN, WHITE, 3, 0 };
	board[3][7] = new ChessPiece{ QUEEN, BLACK, 3, 7 };

	// Place kings
	board[4][0] = new ChessPiece{ KING, WHITE, 4, 0 };
	board[4][7] = new ChessPiece{ KING, BLACK, 4, 7 };
}

void ChessApp::paint() {
	drawBoard();
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] != nullptr) {
				drawPiece(*board[i][j]);
			}
		}
	}
	if (selectedPiece != nullptr) {
		highlightPossibleMoves(*selectedPiece);
	}

	// Display "Check" message if any player is in check
	if (isWhiteInCheck) {
		wout << setpos(winWidth / 2 - 70, winHeight - 18 * (winHeight / 20)) << TEXT("White is in Check!");
	}
	if (isBlackInCheck) {
		wout << setpos(winWidth / 2 - 70, winHeight - 2 * (winHeight / 20)) << TEXT("Black is in Check!");
	}

	// Display "Game Over" message if the game is over
	if (gameOver) {
		setTextColor(red);
		wout << setpos(winWidth / 2 - 50, winHeight / 2) << TEXT("Game Over!");
	}
}

void ChessApp::drawBoard() {
	int grid = 50;
	int offsetX = (winWidth - 8 * grid) / 2;
	int offsetY = (winHeight - 8 * grid) / 2;

	setPen(blue, solidLine);
	for (int y = 0; y <= 8; ++y)
		line(offsetX, offsetY + y * grid, offsetX + 8 * grid, offsetY + y * grid);
	for (int x = 0; x <= 8; ++x)
		line(offsetX + x * grid, offsetY, offsetX + x * grid, offsetY + 8 * grid);
}

void ChessApp::drawPiece(const ChessPiece& piece) {
	int grid = 50;
	int offsetX = (winWidth - 8 * grid) / 2;
	int offsetY = (winHeight - 8 * grid) / 2;
	int pieceX = piece.x * grid + offsetX + grid / 2;
	int pieceY = piece.y * grid + offsetY + grid / 2;

	if (piece.color == WHITE) {
		setPen(gray, solidLine, 2);
		setTextColor(gray);
	}
	else {
		setPen(black, solidLine, 2);
		setTextColor(black);
	}

	circle(Point{ pieceX, pieceY }, grid / 3);

	// Draw the text for the piece
	const TCHAR* pieceName = getPieceName(piece.type);
	wout << setpos(pieceX - 10, pieceY - 10) << pieceName;
}

void ChessApp::highlightPossibleMoves(const ChessPiece& piece) {
	int grid = 50;
	int offsetX = (winWidth - 8 * grid) / 2;
	int offsetY = (winHeight - 8 * grid) / 2;
	std::vector<std::pair<int, int>> moves = getPossibleMoves(piece);
	for (const auto& move : moves) {
		int moveX = move.first * grid + offsetX;
		int moveY = move.second * grid + offsetY;
		if (board[move.first][move.second] != nullptr && board[move.first][move.second]->color != piece.color) {
			setPen(red, solidLine, 2); // Red color for capturing moves
		}
		else {
			setPen(blue, solidLine, 2); // Blue color for regular moves
		}
		rectangle(moveX, moveY, grid, grid);
	}
}

const TCHAR* ChessApp::getPieceName(PieceType type) {
	switch (type) {
	case KING: return TEXT("K");
	case QUEEN: return TEXT("Q");
	case ROOK: return TEXT("R");
	case BISHOP: return TEXT("B");
	case KNIGHT: return TEXT("N");
	case PAWN: return TEXT("P");
	default: return TEXT("");
	}
}

void ChessApp::mouseDown(int x, int y) {
	if (gameOver) return; // Prevent moves if the game is over

	int grid = 50;
	int offsetX = (winWidth - 8 * grid) / 2;
	int offsetY = (winHeight - 8 * grid) / 2;
	int boardX = (x - offsetX) / grid;
	int boardY = (y - offsetY) / grid;

	if (boardX < 0 || boardX >= 8 || boardY < 0 || boardY >= 8) return;

	if (selectedPiece == nullptr && board[boardX][boardY] != nullptr && board[boardX][boardY]->color == currentTurn) {  // Select piece
		selectedPiece = board[boardX][boardY];
	}
	else if (selectedPiece != nullptr) {  // Move piece
		if (isValidMove(selectedPiece->x, selectedPiece->y, boardX, boardY)) {
			movePiece(selectedPiece->x, selectedPiece->y, boardX, boardY);

			// Change king position if the king is moved
			if (selectedPiece->type == KING) {
				if (selectedPiece->color == WHITE) {
					whiteKingX = boardX;
					whiteKingY = boardY;
				}
				else {
					blackKingX = boardX;
					blackKingY = boardY;
				}
			}

			isWhiteInCheck = isInCheck(WHITE);
			isBlackInCheck = isInCheck(BLACK);

			currentTurn = (currentTurn == WHITE) ? BLACK : WHITE; // Change turn
		}
		selectedPiece = nullptr;  // Reset selection
	}
	clearAndUpdate();
}

bool ChessApp::isValidMove(int startX, int startY, int endX, int endY) {
	if (endX < 0 || endX >= 8 || endY < 0 || endY >= 8) return false;
	if (board[endX][endY] != nullptr && board[endX][endY]->color == board[startX][startY]->color) return false;

	// Ensure the move is in the list of possible moves
	std::vector<std::pair<int, int>> moves = getPossibleMoves(*board[startX][startY]);
	for (const auto& move : moves) {
		if (move.first == endX && move.second == endY) {
			return true;
		}
	}
	return false;
}

std::vector<std::pair<int, int>> ChessApp::getPossibleMoves(const ChessPiece& piece) {
	std::vector<std::pair<int, int>> moves;
	int x = piece.x;
	int y = piece.y;

	switch (piece.type) {
	case PAWN:
		if (piece.color == WHITE) {
			if (y + 1 < 8 && board[x][y + 1] == nullptr)
				moves.push_back({ x, y + 1 });
			if (y == 1 && y + 2 < 8 && board[x][y + 1] == nullptr && board[x][y + 2] == nullptr)
				moves.push_back({ x, y + 2 });
			if (x + 1 < 8 && y + 1 < 8 && board[x + 1][y + 1] != nullptr && board[x + 1][y + 1]->color == BLACK)
				moves.push_back({ x + 1, y + 1 });
			if (x - 1 >= 0 && y + 1 < 8 && board[x - 1][y + 1] != nullptr && board[x - 1][y + 1]->color == BLACK)
				moves.push_back({ x - 1, y + 1 });
		}
		else {
			if (y - 1 >= 0 && board[x][y - 1] == nullptr)
				moves.push_back({ x, y - 1 });
			if (y == 6 && y - 2 >= 0 && board[x][y - 1] == nullptr && board[x][y - 2] == nullptr)
				moves.push_back({ x, y - 2 });
			if (x + 1 < 8 && y - 1 >= 0 && board[x + 1][y - 1] != nullptr && board[x + 1][y - 1]->color == WHITE)
				moves.push_back({ x + 1, y - 1 });
			if (x - 1 >= 0 && y - 1 >= 0 && board[x - 1][y - 1] != nullptr && board[x - 1][y - 1]->color == WHITE)
				moves.push_back({ x - 1, y - 1 });
		}
		break;
	case ROOK:
		// Rook movement (straight lines)
		for (int i = x + 1; i < 8; ++i) {
			if (board[i][y] == nullptr) {
				moves.push_back({ i, y });
			}
			else {
				if (board[i][y]->color != piece.color) moves.push_back({ i, y });
				break;
			}
		}
		for (int i = x - 1; i >= 0; --i) {
			if (board[i][y] == nullptr) {
				moves.push_back({ i, y });
			}
			else {
				if (board[i][y]->color != piece.color) moves.push_back({ i, y });
				break;
			}
		}
		for (int i = y + 1; i < 8; ++i) {
			if (board[x][i] == nullptr) {
				moves.push_back({ x, i });
			}
			else {
				if (board[x][i]->color != piece.color) moves.push_back({ x, i });
				break;
			}
		}
		for (int i = y - 1; i >= 0; --i) {
			if (board[x][i] == nullptr) {
				moves.push_back({ x, i });
			}
			else {
				if (board[x][i]->color != piece.color) moves.push_back({ x, i });
				break;
			}
		}
		break;
	case KNIGHT:
		// Knight movement (L shape)
	{
		int knightMoves[8][2] = { {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1} };
		for (auto& move : knightMoves) {
			int newX = x + move[0];
			int newY = y + move[1];
			if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
				if (board[newX][newY] == nullptr || board[newX][newY]->color != piece.color) {
					moves.push_back({ newX, newY });
				}
			}
		}
	}
	break;
	case BISHOP:
		// Bishop movement (diagonals)
		for (int i = 1; i < 8; ++i) {
			if (x + i < 8 && y + i < 8) {
				if (board[x + i][y + i] == nullptr) {
					moves.push_back({ x + i, y + i });
				}
				else {
					if (board[x + i][y + i]->color != piece.color) moves.push_back({ x + i, y + i });
					break;
				}
			}
		}
		for (int i = 1; i < 8; ++i) {
			if (x - i >= 0 && y - i >= 0) {
				if (board[x - i][y - i] == nullptr) {
					moves.push_back({ x - i, y - i });
				}
				else {
					if (board[x - i][y - i]->color != piece.color) moves.push_back({ x - i, y - i });
					break;
				}
			}
		}
		for (int i = 1; i < 8; ++i) {
			if (x + i < 8 && y - i >= 0) {
				if (board[x + i][y - i] == nullptr) {
					moves.push_back({ x + i, y - i });
				}
				else {
					if (board[x + i][y - i]->color != piece.color) moves.push_back({ x + i, y - i });
					break;
				}
			}
		}
		for (int i = 1; i < 8; ++i) {
			if (x - i >= 0 && y + i < 8) {
				if (board[x - i][y + i] == nullptr) {
					moves.push_back({ x - i, y + i });
				}
				else {
					if (board[x - i][y + i]->color != piece.color) moves.push_back({ x - i, y + i });
					break;
				}
			}
		}
		break;
	case QUEEN:
		// Queen movement (combination of Rook and Bishop)
		// Rook part
		for (int i = x + 1; i < 8; ++i) {
			if (board[i][y] == nullptr) {
				moves.push_back({ i, y });
			}
			else {
				if (board[i][y]->color != piece.color) moves.push_back({ i, y });
				break;
			}
		}
		for (int i = x - 1; i >= 0; --i) {
			if (board[i][y] == nullptr) {
				moves.push_back({ i, y });
			}
			else {
				if (board[i][y]->color != piece.color) moves.push_back({ i, y });
				break;
			}
		}
		for (int i = y + 1; i < 8; ++i) {
			if (board[x][i] == nullptr) {
				moves.push_back({ x, i });
			}
			else {
				if (board[x][i]->color != piece.color) moves.push_back({ x, i });
				break;
			}
		}
		for (int i = y - 1; i >= 0; --i) {
			if (board[x][i] == nullptr) {
				moves.push_back({ x, i });
			}
			else {
				if (board[x][i]->color != piece.color) moves.push_back({ x, i });
				break;
			}
		}
		// Bishop part
		for (int i = 1; i < 8; ++i) {
			if (x + i < 8 && y + i < 8) {
				if (board[x + i][y + i] == nullptr) {
					moves.push_back({ x + i, y + i });
				}
				else {
					if (board[x + i][y + i]->color != piece.color) moves.push_back({ x + i, y + i });
					break;
				}
			}
		}
		for (int i = 1; i < 8; ++i) {
			if (x - i >= 0 && y - i >= 0) {
				if (board[x - i][y - i] == nullptr) {
					moves.push_back({ x - i, y - i });
				}
				else {
					if (board[x - i][y - i]->color != piece.color) moves.push_back({ x - i, y - i });
					break;
				}
			}
		}
		for (int i = 1; i < 8; ++i) {
			if (x + i < 8 && y - i >= 0) {
				if (board[x + i][y - i] == nullptr) {
					moves.push_back({ x + i, y - i });
				}
				else {
					if (board[x + i][y - i]->color != piece.color) moves.push_back({ x + i, y - i });
					break;
				}
			}
		}
		for (int i = 1; i < 8; ++i) {
			if (x - i >= 0 && y + i < 8) {
				if (board[x - i][y + i] == nullptr) {
					moves.push_back({ x - i, y + i });
				}
				else {
					if (board[x - i][y + i]->color != piece.color) moves.push_back({ x - i, y + i });
					break;
				}
			}
		}
		break;
	case KING:
		// King movement (one square in any direction)
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0) continue;
				int newX = x + dx;
				int newY = y + dy;
				if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
					if (board[newX][newY] == nullptr || board[newX][newY]->color != piece.color) {
						moves.push_back({ newX, newY });
					}
				}
			}
		}
		break;
	}

	return moves;
}


void ChessApp::movePiece(int startX, int startY, int endX, int endY) {
	// Capture the piece if there is one
	if (board[endX][endY] != nullptr) {
		if (board[endX][endY]->type == KING) {
			gameOver = true; // End the game if a king is captured
		}
		delete board[endX][endY];
	}
	board[endX][endY] = board[startX][startY];
	board[endX][endY]->x = endX;
	board[endX][endY]->y = endY;
	board[startX][startY] = nullptr;
}

bool ChessApp::isInCheck(PieceColor color) {
	int kingX = (color == WHITE) ? whiteKingX : blackKingX;
	int kingY = (color == WHITE) ? whiteKingY : blackKingY;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] != nullptr && board[i][j]->color != color) {
				std::vector<std::pair<int, int>> moves = getPossibleMoves(*board[i][j]);
				for (const auto& move : moves) {
					if (move.first == kingX && move.second == kingY) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

int mainLAF() {
	ChessApp app(TEXT("Chess Game"), 500, 500, 150, 150);
	app.run();
	return 0;
}

int main() {
	mainLAF();
}
