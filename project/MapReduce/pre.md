# MapReduce

```
map(String key, String val):
	// key: ducument
	// val: document contents
	for each word w in val:
		emit(w, "1")

reduce(String key, Iterator values):
	// key: a word
	// values: a list of counts
	int result = 0;
	for each v in values:
		result += ParseInt(v);
	emit(AsString(result));
```

