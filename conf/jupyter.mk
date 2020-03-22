

Pipfile.lock: Pipfile
	( \
		$(PIPENV) install \
	)
jupyter: Pipfile.lock
	( \
		$(PIPENV) run jupyter-lab notebooks/ --allow-root --no-mathjax --ip=0.0.0.0 --port=8888 --no-browser --NotebookApp.token='' \
	)