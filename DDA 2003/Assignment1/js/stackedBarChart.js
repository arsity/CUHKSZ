class StackedAreaChart {

    /**
     * Class constructor with basic chart configuration
     * @param {Array}
     */
    constructor(_data) {
        this.margin = {top: 40, right: 60, bottom: 60, left: 70};
        this.width = 1200 - this.margin.left - this.margin.right;
        this.height = 800 - this.margin.top - this.margin.bottom;
        this.displayType = 'Bar';
        this.data = _data;
        this.flag = false;
        this.initVis();
    }

    /**
     * Initialize scales/axes and append static chart elements
     */
    initVis() {
        let vis = this;


        // Select HTML tag, add an SVG, and set the attributes
        vis.svg = d3
            .select("#bar-chart")
            .append("svg")
            .attr("width", vis.width + vis.margin.left + vis.margin.right)
            .attr("height", vis.height + vis.margin.top + vis.margin.bottom)
            .append("g")
            .attr("transform", `translate(${vis.margin.left}, ${vis.margin.top})`);

        // set bar width
        vis.barWidth = vis.width / vis.data.length;

        // get data groups and sort
        vis.groups = population.map(function (d) {
            return d.State
        });
        vis.sorted_groups = vis.data.total.sort((a, b) => b.Population - a.Population);

        // match data groups to state
        for (let i = 0; i < vis.sorted_groups.length; i++) {
            vis.sorted_groups[i] = vis.sorted_groups[i].State;
        }

        // Create scales for x and y axis
        vis.x = d3.scaleBand()
            .domain(vis.groups)
            .range([0, vis.margin.right + vis.width])
            .padding([0.3]);

        vis.y = d3.scaleLinear()
            .domain([0, max])
            .range([vis.height, 0])


        // Create x and y-axis, create two groups for x and y-axis, and add these groups in SVG
        vis.xAxis = vis.svg.append("g")
            .attr("class", "xAxis")
            .attr("transform", `translate(0, ${vis.height})`)
            .call(d3.axisBottom(vis.x).tickSizeOuter(0));

        vis.yAxis = vis.svg.append("g")
            .call(d3.axisLeft(vis.y));


        // Add y label to the chart
        vis.svg.append("text")
            .attr("class", "ylabel")
            .attr("y", 0 - vis.margin.left + 30)
            .attr("x", 0 - (vis.height / 120))
            .attr("dy", "1em")
            .attr("transform", "rotate(0)")
            .style("text-anchor", "middle")
            .text("Population");

        // Get the population under different age categories and assign color
        vis.subgroups = vis.data.columns.slice(1);

        // default color
        vis.color = d3.scaleOrdinal()
            .range(["#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7"])
            .domain(vis.subgroups)
        // color is represented by hex value (a link for the relationship among different color representations)
        // https://imagecolorpicker.com/color-code
        // Feel free to use your own color set


        // Get stacked data and sort
        // Hint: the following functions will be used during implementing this part:
        //map() and sort() functions in array and d3.stack() for stacked bar chart
        // TODO
        // Prepare data for stacked chart
        vis.stackedData = d3
            .stack()
            .keys(vis.subgroups)
            (vis.data);

    }

    /**
     * Prepare the data and scales before we render it.
     */
    updateVis() {
        let vis = this;

        if (vis.displayType === 'Sorted') {
            vis.x.domain(vis.sorted_groups);
            // sorted_groups is a set of states after sorting by total population
        } else {
            vis.x.domain(vis.groups);
            // groups is a set of states without sorting
        }
        // vis.yScaleFocus.domain([0, d3.max(vis.data, function (d) {
        //     return d.total;
        // })])

        vis.renderVis();
    }

    /**
     * This function contains the D3 code for binding data to visual elements
     * Important: the chart is not interactive yet and renderVis() is intended
     * to be called only once; otherwise new paths would be added on top
     */
    renderVis() {

        let vis = this;
        vis.svg.selectAll(".rects").remove() // clear rectangles
        vis.svg.selectAll(".xAxis").remove() // clear x-axis group
        vis.svg.selectAll(".legend").remove() // clear legend

        // control colors
        if (vis.displayType === 'Bar') {
            // Visualize Bar Chart
            vis.color.range(["#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7", "#8dd3c7"]);
        } else {
            // Visualize Stacked Bar Chart
            vis.color.range(["#8dd3c7", "#ffffb3", "#bebada", "#fb8072", "#80b1d3", "#fdb462", "#b3de69"]);
        }

        // draw bars
        vis.svg.append("g")
            .attr("class", "rects")
            .selectAll("g")
            .data(vis.stackedData)
            .join("g")
            .attr("fill", d => vis.color(d.key))
            .selectAll("rect")
            .data(d => d)
            .join("rect")
            .attr("x", d => vis.x(d.data.State))
            .attr("y", d => vis.y(d[1]))
            .attr("height", d => vis.y(d[0]) - vis.y(d[1]))
            .attr("width", vis.x.bandwidth())

        // vis.x.call(vis.xAxis);
        // vis.y.call(vis.yAxis);

        // draw x-axis groups
        vis.xAxis = vis.svg.append("g")
            .attr("class", "xAxis")
            .attr("transform", `translate(0, ${vis.height})`)
            .call(d3.axisBottom(vis.x).tickSizeOuter(0));


        // Display legend
        if (vis.displayType !== 'Bar') {
            let legend = vis.svg.append("g")
                .attr("class", "legend")
                .attr("font-size", 10)
                .attr("text-anchor", "end")
                .selectAll("g")
                .data(vis.subgroups.slice())
                .enter().append("g")
                .attr("transform", function (d, i) {
                    return "translate(0," + i * 20 + ")";
                });

            legend.append("rect")
                .attr("class", "legend")
                .attr("x", vis.width)
                .attr("width", 19)
                .attr("height", 19)
                .data(vis.stackedData)
                .attr("fill", d => vis.color(d.key));

            legend.append("text")
                .attr("class", "legend")
                .attr("x", vis.width)
                .attr("y", 9.5)
                .attr("dy", "0.32em")
                .text(function (d) {
                    return d;
                });
        }
    }
}