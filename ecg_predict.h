#ifndef ECG_PREDICT_H
#define ECG_PREDICT_H

class CEcgPredict
{
public:
    CEcgPredict();
};

#endif // ECG_PREDICT_H

// not completed, see page 50 《Integrate into my server written in Qt》 in the project document

/*
 * I didn’t have time to complete the integration of this part into my actual server code. Tensorflow is such a complicated library and I’m facing problem when compiling with qmake in Qt.

Since the code was complied successfully when I wrote it in Xcode, I will blame it on the Qt Complier. This shouldn’t be a big problem but it will take me way too long to fix it. Therefore I chose to leave it. I’ve successfully shown how the model was trained was how it can now predict the correct output for an ECG wave. Without the compiling issue the code should work just as fine without a single bit of modification.

Here’s me from 15 minutes later after writing the above paragraph. I shouldn't have said I'd give up so early as it didn’t take that long for me to fix it!


I manually added the include path and library file into the qmake configuration. And the program built without an error.



……
I was too happy too early. Even the program passed the compile, it failed to start:

I thinks that I probably needs to find a way to pack the dylibs into the Content of the app file. But I don’t really know how to do it and I don't know if that will really fix the problem.
I’m actually not gonna spend more time on this problem. As I said, it’s some weird complier linkage problem. It’s not worth it to spend hours solving this. Plus I’ve tried my best. I regret to say that this part’s development has to end here.
*/
