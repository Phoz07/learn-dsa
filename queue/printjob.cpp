#include <iostream>
#include <queue>
#include <string>

class PrintJob {
public:
  std::string documentName;
  int numberOfPages;

  PrintJob(std::string docName, int pages)
      : documentName(docName), numberOfPages(pages) {}
};

class PrinterQueue {
private:
  std::queue<PrintJob> queue;

public:
  void addJob(const PrintJob &job) {
    queue.push(job);
    std::cout << "Added to queue: " << job.documentName << " ("
              << job.numberOfPages << " pages)" << std::endl;
  }

  void processJobs() {
    while (!queue.empty()) {
      PrintJob currentJob = queue.front();
      std::cout << "Printing: " << currentJob.documentName << " ("
                << currentJob.numberOfPages << " pages)" << std::endl;
      // Simulate printing delay
      queue.pop(); // Remove the job from the queue after printing
    }
    std::cout << "All print jobs completed." << std::endl;
  }
};

int main() {
  PrinterQueue printer;

  printer.addJob(PrintJob("Document1.pdf", 10));
  printer.addJob(PrintJob("Document2.pdf", 5));
  printer.addJob(PrintJob("Document3.pdf", 12));

  printer.processJobs(); // Start processing print jobs

  return 0;
}
