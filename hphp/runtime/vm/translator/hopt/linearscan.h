/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef incl_HPHP_VM_LINEAR_SCAN_H_
#define incl_HPHP_VM_LINEAR_SCAN_H_

#include "runtime/vm/translator/hopt/state_vector.h"

namespace HPHP { namespace VM { namespace JIT {

class Trace;
class IRFactory;

struct UseInfo {
  UseInfo() : lastUse(0), count(0) {}
  uint32_t lastUse; // linear id of last use
  uint32_t count;   // number of uses
};

typedef StateVector<IRInstruction, uint32_t> LinearIdVector;
typedef StateVector<SSATmp, UseInfo> UsesVector;

struct LifetimeInfo {
  explicit LifetimeInfo(const IRFactory* factory)
    : linear(factory, 0), uses(factory, UseInfo()) {
  }
  explicit LifetimeInfo(const LinearIdVector& linear,
                        const UsesVector& uses)
    : linear(linear), uses(uses) {
  }
  explicit LifetimeInfo(LinearIdVector&& linear,
                        UsesVector&& uses)
    : linear(linear), uses(uses) {
  }

  LinearIdVector linear; // linear id of each instruction
  UsesVector uses;       // last use id and use count of each tmp
};

/*
 * The main entry point for register allocation.  Called prior to code
 * generation.
 */
void allocRegsForTrace(Trace*, IRFactory*, LifetimeInfo* lifetime = nullptr);

}}}

#endif
