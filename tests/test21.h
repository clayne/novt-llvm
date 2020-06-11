#include <utility>
#include <stdint.h>
#include "includes/llvm_arrayref.h"

namespace llvm {
  struct raw_ostream{};

namespace mca {

struct InstRef{};
struct HWInstructionEvent{};
struct HWStallEvent{};
struct HWPressureEvent{};

class HWEventListener {
public:
  // Generic events generated by the pipeline.
  virtual void onCycleBegin() {}
  virtual void onCycleEnd() {}

  virtual void onEvent(const HWInstructionEvent &Event) {}
  virtual void onEvent(const HWStallEvent &Event) {}
  virtual void onEvent(const HWPressureEvent &Event) {}

  using ResourceRef = std::pair<uint64_t, uint64_t>;
  virtual void onResourceAvailable(const ResourceRef &RRef) {}

  // Events generated by the Scheduler when buffered resources are
  // consumed/freed for an instruction.
  virtual void onReservedBuffers(const InstRef &Inst,
                                 ArrayRef<unsigned> Buffers) {}
  virtual void onReleasedBuffers(const InstRef &Inst,
                                 ArrayRef<unsigned> Buffers) {}

  virtual ~HWEventListener() {}

private:
  virtual void anchor();
};



class View : public HWEventListener {
public:
  virtual void printView(llvm::raw_ostream &OS) const = 0;
  virtual ~View() = default;
  void anchor() override;
};



class SummaryView : public View {
  unsigned TotalCycles = 0;

public:
  SummaryView();

  void onCycleBegin() override {}
  void onCycleEnd() override { ++TotalCycles; }
  void onEvent(const HWInstructionEvent &Event) override;
  void printView(llvm::raw_ostream &OS) const override;

  /*void onEvent(const HWStallEvent &Event) override {}
  void onEvent(const HWPressureEvent &Event) override {}

  void onResourceAvailable(const ResourceRef &RRef) override {}
  void onReservedBuffers(const InstRef &Inst, ArrayRef<unsigned> Buffers) override {}
  void onReleasedBuffers(const InstRef &Inst, ArrayRef<unsigned> Buffers) override {}*/

  virtual ~SummaryView() = default;
};

} // namespace mca
} // namespace llvm