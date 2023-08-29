#include "sheet.h"

#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text) {

    /*if (!pos.IsValid()) {
        throw InvalidPositionException("invalid cell position in SetCell");
        //throw FormulaException("invalid cell position in SetCell");
    }

    cells_.resize(std::max(pos.row+1, static_cast<int>(std::size(cells_))));
    cells_[pos.row].resize(std::max(pos.col+1, static_cast<int>(std::size(cells_[pos.row]))));

    if (!cells_[pos.row][pos.col]) {
        cells_[pos.row][pos.col] = std::make_unique<Cell>(*this);
    }

    cells_[pos.row][pos.col]->Set(std::move(text));*/

    if (pos.IsValid()) {

           cells_.resize(std::max(pos.row + 1, int(std::size(cells_))));
           cells_[pos.row].resize(std::max(pos.col + 1, int(std::size(cells_[pos.row]))));

           if (!cells_[pos.row][pos.col]) {cells_[pos.row][pos.col] = std::make_unique<Cell>(*this);}
           cells_[pos.row][pos.col]->Set(std::move(text));

       } else {
           throw InvalidPositionException("invalid cell position. setsell");
       }
}

CellInterface* Sheet::GetCell(Position pos) {

    if (!pos.IsValid()) {
        throw InvalidPositionException("invalid cell position in GetCell");
    }

    if (pos.row < static_cast<int>(std::size(cells_)) && pos.col < static_cast<int>(std::size(cells_[pos.row]))) {

        if (cells_[pos.row][pos.col].get()->GetText() == "") {
            return nullptr;
        } else {
            return cells_[pos.row][pos.col].get();
        }

    } else {
        return nullptr;
    }
}

const CellInterface* Sheet::GetCell(Position pos) const {

    if (!pos.IsValid()) {
        throw InvalidPositionException("invalid cell position in const GetCell");
    }

    if (pos.row < static_cast<int>(std::size(cells_)) && pos.col < static_cast<int>(std::size(cells_[pos.row]))) {

        if (cells_[pos.row][pos.col].get()->GetText() == "") {
            return nullptr;
        } else {
            return cells_[pos.row][pos.col].get();
        }

    } else {
        return nullptr;
    }
}

Cell* Sheet::GetCellPointer(Position pos) {

    if (!pos.IsValid()) {
        throw InvalidPositionException("invalid cell position in GetCellPointer");
    }
    if (pos.row < int(std::size(cells_)) && pos.col < int(std::size(cells_[pos.row]))) {
        return cells_[pos.row][pos.col].get();

    } else {
        return nullptr;
    }
}

const Cell* Sheet::GetCellPointer(Position pos) const {
    const Cell* result = GetCellPointer(pos);
    return result;
}

void Sheet::ClearCell(Position pos) {

    if (!pos.IsValid()) {
        throw InvalidPositionException("invalid cell position in ClearCell");
    }

    if (pos.row < static_cast<int>(std::size(cells_)) && pos.col < static_cast<int>(std::size(cells_[pos.row]))) {
        if (cells_[pos.row][pos.col]) {
            cells_[pos.row][pos.col]->Clear();
        }
    }
}

Size Sheet::GetPrintableSize() const {

    Size result;

    for (auto a = 0; a < static_cast<int>(std::size(cells_)); ++a) {
        for (auto b = static_cast<int>(std::size(cells_[a]) - 1); b >= 0 ; --b) {
            if (cells_[a][b]) {
                if (cells_[a][b]->GetText() == "") {
                    continue;
                }
                else {
                    result.rows = std::max(result.rows, a + 1);
                    result.cols = std::max(result.cols, b + 1);
                    break;
                }
            }
        }
    }

    return result;
}

void Sheet::PrintValues(std::ostream& output) const {
    for (int a = 0; a < GetPrintableSize().rows; ++a) {
        for (int b = 0; b < GetPrintableSize().cols; ++b) {
            if (b > 0) {
                output << '\t';
            }

            if (b < int(std::size(cells_[a]))) {
                if (cells_[a][b]) {std::visit([&output](const auto& obj){output << obj;},
                                       cells_[a][b]->GetValue());}
            }
        }
        output << '\n';
    }
}

void Sheet::PrintTexts(std::ostream& output) const {
    for (int a = 0; a < GetPrintableSize().rows; ++a) {
        for (int b = 0; b < GetPrintableSize().cols; ++b) {

            if (b) {output << '\t';}

            if (b < static_cast<int>(std::size(cells_[a]))) {
                if (cells_[a][b]) {output << cells_[a][b]->GetText();}
            }
        }
        output << '\n';
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}
