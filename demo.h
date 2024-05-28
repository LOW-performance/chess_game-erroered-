#ifndef DEMO_H
#define DEMO_H

#include "pclaf.h"
#include <iostream>
#include <vector>
#include <cstring> // memset 사용을 위해 추가

// Customized Application
enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };
enum PieceColor { WHITE, BLACK };

struct ChessPiece {
	PieceType type;
	PieceColor color;
	int x, y;  // Board coordinates
	ChessPiece(PieceType t, PieceColor c, int xPos, int yPos) : type(t), color(c), x(xPos), y(yPos) {}
};

class ChessApp : public Application {
	ChessPiece* board[8][8];  // 체스판
	ChessPiece* selectedPiece; // 선택된 체스말
	PieceColor currentTurn; // 현재 턴
	int winWidth, winHeight;
	int mouseX, mouseY;

	// King positions
	int whiteKingX, whiteKingY;
	int blackKingX, blackKingY;

	// Check status
	bool isWhiteInCheck;
	bool isBlackInCheck;

	// Game over flag
	bool gameOver;

public:
	ChessApp(const TCHAR* apName, int width, int height, int x, int y)
		: Application(apName, width, height, x, y), selectedPiece(nullptr), currentTurn(BLACK), winWidth(width), winHeight(height), mouseX(-1), mouseY(-1), whiteKingX(4), whiteKingY(0), blackKingX(4), blackKingY(7), isWhiteInCheck(false), isBlackInCheck(false), gameOver(false) {
		memset(board, 0, sizeof(board));
		initializeBoard();
	}

	~ChessApp() {
		// 체스말 객체 해제
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				delete board[i][j];
			}
		}
	}

	void initializeBoard();
	void paint() override;
	void drawBoard();
	void drawPiece(const ChessPiece& piece);
	void highlightPossibleMoves(const ChessPiece& piece);

	void mouseDown(int x, int y) override;

	bool isValidMove(int startX, int startY, int endX, int endY);
	std::vector<std::pair<int, int>> getPossibleMoves(const ChessPiece& piece);
	void movePiece(int startX, int startY, int endX, int endY);
	const TCHAR* getPieceName(PieceType type);
	bool isInCheck(PieceColor color);  // 새로운 함수
};

#endif