# Progress Report: C++ Web Crawler

## 1. Webpage Downloader

I have successfully implemented the webpage downloader component using the `libcurl` library. The `downloadWebpage` function, which I created, takes a URL as input, sends an HTTP request, and retrieves the webpage content. I have also added robust error handling to gracefully deal with any exceptions that may occur during the download process, ensuring the stability and reliability of the crawler.

## 2. Link Extractor

To enable the crawler to discover new pages, I have developed the link extractor component. This component is responsible for extracting links from the downloaded webpage content. I utilized regular expressions to accurately identify and extract links embedded in the HTML. The extracted links are efficiently stored in a `vector` data structure for further processing. Additionally, I implemented functions to normalize links and extract the domain from a URL, which helps in determining the relevance and scope of the discovered links.

For example, I used the regular expression `"<a\\s+(?:[^>]*?\\s+)?href=([\"'])(.*?)\\1"` to match and extract the links from the HTML content. This regular expression carefully handles various link formats and attributes, ensuring comprehensive link extraction.

## 3. Data Structure for Storing Visited Webpages

To keep track of the webpages that have already been visited, I implemented two data structures: `SetList` and `StripedHashSet`.

- `SetList` is a straightforward data structure that utilizes an `unordered_set` to store the visited webpages. It provides `contains` and `insert` functions, allowing efficient checking and addition of webpages to the set. This data structure ensures that the crawler avoids revisiting already processed pages, optimizing the crawling process.

- `StripedHashSet` is a more advanced data structure that I designed to handle concurrent access in a multithreaded environment. It is implemented using a vector of `unordered_set`s, where each set represents a stripe. By employing striped locking, I ensure thread safety and minimize contention. The `contains` and `insert` functions of `StripedHashSet` use shared locks and unique locks, respectively, to achieve efficient and safe concurrent access to the visited webpage data.

## 4. Multithreaded Crawler

To harness the power of parallel processing and improve the efficiency of the crawler, I have begun the implementation of the multithreaded crawler. The `Crawler` class, which I developed, serves as the central orchestrator of the crawling process. It manages the URL queue, visited webpages, and website index. The `run` function, a key component of the `Crawler` class, is responsible for starting the specified number of threads. Each thread independently executes the `crawlLoop` function, which retrieves URLs from the queue, downloads the webpage, extracts links, and adds new URLs to the queue for further exploration.

To ensure thread safety and coordination, I have employed synchronization primitives such as mutexes and condition variables. For instance, the following code snippet demonstrates the use of a unique lock and a condition variable to wait until the URL queue is not empty before proceeding with the crawling process:

```cpp
std::unique_lock<std::mutex> lock(queueMutex);
queueCondVar.wait(lock, [this] { return !urlQueue.empty(); });
```

By leveraging these synchronization mechanisms, I guarantee that the multithreaded crawler operates seamlessly and avoids race conditions or data inconsistencies.

Throughout the development process, I have made significant strides in implementing the essential components of the web crawler. From the webpage downloader and link extractor to the data structures for storing visited webpages and the initial implementation of the multithreaded crawler, I have laid a solid foundation for a robust and efficient crawling system.

Moving forward, my focus will be on further enhancing the crawler's functionality, handling edge cases, and optimizing performance. I will continue to refine the implemented components, address any identified issues, and explore additional features to make the crawler more versatile and effective.