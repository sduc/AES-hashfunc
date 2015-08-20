FNAME = "perf_stats.txt"

if __name__ == "__main__":
    f = open(FNAME,'r')
    lines = f.readlines()
    plist = [(log(int(l.split(':')[0],10)),float(l.split(':')[1])) for l in lines]
    plot = list_plot(plist)
    plot.axes_labels(['$\log(bytelen(msg))$','$CPU cycles/byte$'])
    plot.set_legend_options(title="Cycles per bytes needed to hash a msg of x bytes")
    plot.save('plot_perf_stats.eps')
    plot.save('plot_perf_stats.png')
