.PHONY: clean All

All:
	@echo "----------Building project:[ TileTool - Debug ]----------"
	@cd "TileTool" && "$(MAKE)" -f  "TileTool.mk"
clean:
	@echo "----------Cleaning project:[ TileTool - Debug ]----------"
	@cd "TileTool" && "$(MAKE)" -f  "TileTool.mk" clean
