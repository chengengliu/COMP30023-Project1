This is Projec1 for COMP30023

This is the day I start the project (Start reading through the spec lol )

Nothing special to state about the project.
Programming is fun! Have a nice day!

/*
from Chris  about testing multithreading:
Following an email question regarding how to test whether your server is correctly using multiple threads, I thought a short guide might be useful.

One of the easiest ways of testing your multithreaded server is to create two requests, an initial one that requests a large file and downloads it slowly, and then a second, concurrent request, for a small file. If the second request is responded to before the first one finishes it shows there must be some form of concurrency/multithreading taking place. This is easy to do with standard Linux tools, in particular wget. You can limit the download speed of wget using the --limit-rate argument https://www.gnu.org/software/wget/manual/wget.html.

You will need to create a large file to test this with, in particular, the file needs to be larger than the send buffers on the socket. The reason for this is because we want to cause the write operation on the slow download to block. If the file is smaller than the send buffer it could be written entirely into the buffer and the process would continue even though the data hadn't actually been sent. This could create the impression of concurrency where there isn't any. You can find the default size of the send buffer by checking the wmem_default system value (hint: cat /proc/sys/net/core/wmem_default).

Picking the right test file size requires some care. The easiest way is to multiply the size of the send buffer by the number of seconds you need to perform a second request, plus one. For example, if your buffer is 212992, and you want 10 seconds to test a second request, you need a test file of 11*212992 = 2342912 bytes. Then set the download rate equal to the buffer size (212992). This will give you 10 seconds of time to test a second request, whilst still completing the original request in a reasonable amount of time (11 seconds).

Once you have your file ready, run an initial wget for your large file with --limit-rate set to the buffer size. Then run a second instance of wget (without any rate limit) for another, smaller test file. The second test file should be returned whilst the first is still being downloaded. If both succeed you have good evidence that the multi-threading is working correctly. To run multiple instances of wget you can run it in the background using the -b option, but the easiest way is to create multiple ssh sessions in different terminals so you can keep track of each request's progress.
*/
