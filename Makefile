CC=g++

buildntest: build test

build:
	$(CC) ingest.cpp -o ingest

clean:
	rm ingest

test:
	./ingest sonar_logs/Sonar0001.sl2 ingested_data.csv
