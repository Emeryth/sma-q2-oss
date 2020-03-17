#protobuf files
SRCPB = $(shell find $(SRC_DIR) -maxdepth 2 \( -iname "*.proto" \))
PBMODELS = $(patsubst $(SRC_DIR)%.proto,$(SRC_DIR)%.pb.c,$(SRCPB) )
PB_OBJS = $(patsubst $(SRC_DIR)%.proto,$(BUILD_DIR)$(SRC_DIR)%.pb.c.o,$(SRCPB) )
PROTOC = protoc

# protocol buffer models
src/protobuff/%.pb.c:: $(SRCPB) Pipfile.lock
	$(PIPENV) run $(PROTOC) --plugin=protoc-gen-nanopb=./lib/nanopb/generator/protoc-gen-nanopb --nanopb_out=. $<
	$(PIPENV) run $(PROTOC) --python_out=. $<
	$(FIND) src/protobuff -name "*.pb.c" -exec $(SED) 's|src/protobuff/||' {} \;
	$(MV) src/protobuff/*_pb2.py notebooks