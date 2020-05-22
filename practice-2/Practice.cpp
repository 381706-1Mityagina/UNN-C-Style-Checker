#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
    struct Practice : public FunctionPass {
        static char ID;
        Practice() : FunctionPass(ID) {}

        bool runOnFunction(Function& function) override {
            errs() << "********** PRACTICE PEEPHOLE OPTIMIZER **********\n";
            errs() << "********** Function: " << function.getName() << '\n';

            bool changed = false;

            // итератор на начало и конец функции ~ как по контейнеру к-л
            for (inst_iterator ii = inst_begin(function), ie = inst_end(function); ii != ie;) {
                // избегаем SegFault.
                // Нужно будет переходить к следующей инструкции,
                // ссылку на которую хранит текущая. И при ее(current)
                // удалении -> беда 
                auto instruction = &*ii;
                ++ii;

                // понимаем, что нашли бесполезную инструкцию
                if (!isTrivialSub(instruction)) {
                    continue;
                }
                // замена
                if (!instruction->use_empty()) {
                    replaceWithArgument(instruction);
                }
                // удаляем
                instruction->eraseFromParent();
                changed = true;
            }
            return changed;
        }

    private:
        bool isTrivialSub(Instruction const* instruction) const {
            return isBinarySub(instruction) && isTrivial(instruction);
        }

        bool isBinarySub(Instruction const* instruction) const {
            return instruction->getOpcode() == Instruction::Sub && instruction->getNumOperands() == 2;
        }

        bool isTrivial(Instruction const* instruction) const {
            auto const& operands = instruction->operands();
            return std::any_of(operands.begin(), operands.end(), [this](auto const& operand) { return isZero(operand); });
        }

        // check: является ли константой времени компиляции и = 0
        bool isZero(Value const* operand) const {
            // dyn_cast - более общая штука, нежели стандартный dynamic_cast
            auto const constant = dyn_cast<ConstantInt>(operand);
            return constant && constant->isZero();
        }

        void replaceWithArgument(Instruction* instruction) const {
            // меняем на ненулевой аргумент
            auto const& lhs = instruction->getOperand(0);
            auto const& rhs = instruction->getOperand(1);
            instruction->replaceAllUsesWith(isZero(lhs) ? rhs : lhs);
        }
    };
}

// Регистрируем Pass :
// Этот шаблонный класс используется для уведомления системы о том, что Pass доступен для использования,
// и регистрирует его во внутренней базе данных, поддерживаемой PassManager. Если этот шаблон не используется, 
// например, opt не сможет увидеть Pass, и попытки создать Pass не удастся.
char Practice::ID = 0;
static RegisterPass<Practice> X(
    "practice",
    "Practice Peephole Optimization Pass",
    false,
    false
);

// Это должно использоваться плагинами оптимизатора, чтобы позволить всем интерфейсам использовать их (Passes)
static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder& builder, legacy::PassManagerBase& manager) { manager.add(new Practice()); }
);

