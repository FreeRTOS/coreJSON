# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: MIT-0

PROOF_UID=skipCollection
CHECK_FUNCTION_CONTRACTS = $(PROOF_UID)
HARNESS_FILE= $(PROOF_UID)_harness

CBMC_OBJECT_BITS = 10

# This value was experimentally chosen to provide 100% coverage
# without tripping unwinding assertions and without exhausting memory.
CBMC_MAX_BUFSIZE=10

DEFINES += -DJSON_MAX_DEPTH="( $(CBMC_MAX_BUFSIZE) - 2 )"

USE_FUNCTION_CONTRACTS += skipScalars
USE_FUNCTION_CONTRACTS += skipSpace

UNWINDSET += skipCollection.0:$(CBMC_MAX_BUFSIZE)
UNWINDSET += skipCollection_wrapped_for_contract_checking.0:$(CBMC_MAX_BUFSIZE)

include ../Makefile-json.common

# Substitution command to pass to sed for patching core_json.c. The
# characters " and # must be escaped with backslash.
CORE_JSON_SED_EXPR = s/^static //
