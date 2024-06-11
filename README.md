# Web Crawler

This is my web crawler project.
To run, simply make the project and run the executable.
Make with: `make main`
Run with: `./main`

Then input the URL you want to crawl.
The program will automatically go through different thread counts and find the optimal thread count for the given URL.

There is also `grapher.py`, that allows you to load the output of the program and graph it to a force-directed graph. Install `requirements.txt`, rename the target file to `sitemap.json` and run `python3 grapher.py`. It will then create `force_directed_graph.html` which you can open in your browser to see the graph.
