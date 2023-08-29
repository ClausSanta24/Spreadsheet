#pragma once

#include "cell.h"
#include "common.h"

#include <functional>

class Sheet : public SheetInterface {
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    CellInterface* GetCell(Position pos) override;
    const CellInterface* GetCell(Position pos) const override;

    Cell* GetCellPointer(Position pos);
    const Cell* GetCellPointer(Position pos) const;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

	// Можете дополнить ваш класс нужными полями и методами


private:
	// Можете дополнить ваш класс нужными полями и методами

    std::vector<std::vector<std::unique_ptr<Cell>>> cells_;
};
