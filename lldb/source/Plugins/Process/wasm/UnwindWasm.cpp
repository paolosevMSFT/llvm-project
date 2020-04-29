//===-- UnwindWasm.cpp ----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnwindWasm.h"
#include "Plugins/Process/wasm/ProcessWasm.h"

using namespace lldb;
using namespace lldb_private;
using namespace process_gdb_remote;
using namespace wasm;

lldb::RegisterContextSP
UnwindWasm::DoCreateRegisterContextForFrame(lldb_private::StackFrame *frame) {
  if (m_frames.size() <= frame->GetFrameIndex()) {
    return lldb::RegisterContextSP();
  }

  IWasmProcess *wasm_process =
      static_cast<WasmProcessGDBRemote *>(frame->CalculateProcess().get());
  return wasm_process->CreateRegisterContextForFrame(
      frame, m_frames[frame->GetFrameIndex()]);
}

uint32_t UnwindWasm::DoGetFrameCount() {
  if (!m_unwind_complete) {
    m_unwind_complete = true;
    m_frames.clear();

    IWasmProcess *wasm_process =
        static_cast<WasmProcessGDBRemote *>(GetThread().GetProcess().get());
    if (wasm_process)
      if (!wasm_process->GetWasmCallStack(m_frames))
        m_frames.clear();
  }
  return m_frames.size();
}

bool UnwindWasm::DoGetFrameInfoAtIndex(uint32_t frame_idx, lldb::addr_t &cfa,
                                       lldb::addr_t &pc,
                                       bool &behaves_like_zeroth_frame) {
  if (m_frames.size() == 0) {
    DoGetFrameCount();
  }

  if (frame_idx < m_frames.size()) {
    behaves_like_zeroth_frame = (frame_idx == 0);
    cfa = 0;
    pc = m_frames[frame_idx];
    return true;
  }
  return false;
}