# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

HARNESS_FILE=JSON_Validate_harness
PROOF_UID=JSON_Validate

# This value was experimentally chosen to provide 100% coverage
# without tripping unwinding assertions and without exhausting memory.
CBMC_MAX_BUFSIZE=14

UNWINDSET += JSON_Validate.0:$(CBMC_MAX_BUFSIZE)

PROOF_SOURCES += $(PROOF_STUB)/skipGeneric.c
PROOF_SOURCES += $(PROOF_STUB)/skipAnyLiteral.c
PROOF_SOURCES += $(PROOF_STUB)/skipCollection.c
PROOF_SOURCES += $(PROOF_STUB)/skipNumber.c
PROOF_SOURCES += $(PROOF_STUB)/skipSpace.c
PROOF_SOURCES += $(PROOF_STUB)/skipString.c

include ../Makefile-json.common

$(PROOFDIR)/core_json.c: $(SRCDIR)/source/core_json.c
	sed -E '1s/^/#include "core_json_annex.h" /; s/^static //; s/(bool_|JSONStatus_t|void) skip(AnyLiteral|Collection|Number|Space|String)\b/&_/' $< > $@
