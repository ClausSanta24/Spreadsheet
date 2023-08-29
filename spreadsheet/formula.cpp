#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << "#DIV/0!";
}

namespace {
class Formula : public FormulaInterface {
public:
// Реализуйте следующие методы:

    explicit Formula(std::string expression) try
        : ast_(ParseFormulaAST(expression))
    {}
    catch (...) {
        throw FormulaException("Incorrect formula");
    }

    Value Evaluate(const SheetInterface& sheet) const override {

        try {

            std::function<double(Position)> args = [&sheet](const Position pos)->double {

                if (pos.IsValid()) {

                    const auto* cell = sheet.GetCell(pos);
                    if (cell) {

                        if (std::holds_alternative<double>(cell->GetValue())) {
                            return std::get<double>(cell->GetValue());

                        } else if (std::holds_alternative<std::string>(cell->GetValue())) {

                            auto str_value = std::get<std::string>(cell->GetValue());
                            if (str_value != "") {
                                std::istringstream input(str_value);
                                double num = 0.0;

                                if (input.eof() && input >> num) {
                                    return num;
                                } else {
                                    throw FormulaError(FormulaError::Category::Value);
                                }

                            } else {
                                return 0.0;
                            }

                        } else {
                            throw FormulaError(std::get<FormulaError>(cell->GetValue()));
                            //throw FormulaError(FormulaError::Category::Div0);
                        }

                    } else {
                        return 0.0;
                    }

                } else {
                    throw FormulaError(FormulaError::Category::Ref);
                }
            };

            return ast_.Execute(args);

        } catch (const FormulaError& evaluate_error) {
            return evaluate_error;
        }
    }

    std::string GetExpression() const override {
        std::ostringstream str;
        ast_.PrintFormula(str);
        return str.str();
    }

    std::vector<Position> GetReferencedCells() const override {
        std::vector<Position> result;
        for (const auto& cell : ast_.GetCells()) {
           if (cell.IsValid()) {
               result.push_back(cell);
           }
        }
        return result;
    }

private:
    FormulaAST ast_;
};
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}
