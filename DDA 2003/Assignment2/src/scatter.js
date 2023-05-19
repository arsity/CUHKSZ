async function drawScatter() {

    // 1. Access data

    const dataset = await d3.json("./data/my_weather_data.json")
    // set data constants

    // Get data attributes, i.e. xAccessor for min temperature and yAccessor for max temperature

    const xAccessor = function (d) {
        return d.temperatureMin
    }
    const yAccessor = function (d) {
        return d.temperatureMax
    }

    // const colorScaleYear = 2000

    const parseDate = d3.timeParse("%Y-%m-%d") // time string to time object
    const dateFormat = d3.timeFormat("%a, %d %b, %Y") // time object to time string of plot
    const dateFormat_legend = d3.timeFormat("%d %b") // time object to time string of legend

    const dateFormatter = function (d) {
        return dateFormat(parseDate(d.date))
    }

    const colorAccessor = function (d) {
        return parseDate(d.date)
    }

    // Create chart dimensions

    const width = d3.min([window.innerWidth * 0.75, window.innerHeight * 0.75,])
    let dimensions = {
        width: width, height: width, margin: {
            top: 90, right: 90, bottom: 50, left: 50,
        }, legendWidth: 250, legendHeight: 26, // set marginal histogram
        histogramMargin: 10, histogramHeight: 70
    }
    dimensions.boundedWidth = dimensions.width - dimensions.margin.left - dimensions.margin.right
    dimensions.boundedHeight = dimensions.height - dimensions.margin.top - dimensions.margin.bottom

    // Draw

    const wrapper = d3.select("#wrapper")
        .append("svg")
        .attr("width", dimensions.width)
        .attr("height", dimensions.height)

    const bounds = wrapper.append("g")
        .style("transform", `translate(${dimensions.margin.left}px, ${dimensions.margin.top}px)`)

    const boundsBackground = bounds.append("rect")
        .attr("class", "bounds-background")
        .attr("x", 0)
        .attr("width", dimensions.boundedWidth)
        .attr("y", 0)
        .attr("height", dimensions.boundedHeight)

    // Create scales

    // Create scales for x, y, and color (i.e., xScale, yScale, and colorScale)

    let xScale = d3
        .scaleLinear()
        .domain([0, 100])
        .range([0, dimensions.boundedWidth])

    let yScale = d3
        .scaleLinear()
        .domain([0, 100])
        .range([dimensions.boundedHeight, 0])

    let colorScale = d3.scaleSequential()
        .domain(d3.extent(dataset, function (d) {
            return colorAccessor(d)
        }))
        .interpolator(function (d) {
            return d3.interpolateRainbow(-d)
        })

    // 5. Draw data

    // draw data into a scatter plot

    const dotsGroup = bounds.append("g");

    const dots = dotsGroup
        .selectAll("circle")
        .data(dataset)
        .join("circle")
        .attr("class", "dot")
        .attr("r", 3.5)
        .attr("cx", function (d) {
            return xScale(xAccessor(d))
        })
        .attr("cy", function (d) {
            return yScale(yAccessor(d))
        })
        .style("fill", function (d) {
            return colorScale(colorAccessor(d))
        })

    // draw top marginal histogram

    const topHistogram_generator = d3
        .histogram()
        .domain(xScale.domain())
        .value(xAccessor)
        .thresholds(20)
    // play around with the number of thresholds

    const topHistogramBins = topHistogram_generator(dataset)

    const topHistogramYScale = d3
        .scaleLinear()
        .domain(d3.extent(topHistogramBins, function (d) {
            return d.length
        }))
        .range([dimensions.histogramHeight, 0])

    const topHistogramBounds = bounds
        .append("g")
        .attr("transform", `translate(0, ${-dimensions.histogramHeight - dimensions.histogramMargin})`)

    const topHistogramLineGenerator = d3
        .area()
        .x(function (d) {
            return xScale((d.x0 + d.x1) / 2)
        })
        .y0(dimensions.histogramHeight)
        .y1(function (d) {
            return topHistogramYScale(d.length)
        })
        .curve(d3.curveBasis)

    const topHistogramElement = topHistogramBounds
        .append("path")
        .attr("d", function (d) {
            return topHistogramLineGenerator(topHistogramBins)
        })
        .attr("class", "histogram-area")

    const rightHistogramGenerator = d3
        .histogram()
        .domain(yScale.domain())
        .value(yAccessor)
        .thresholds(20)

    const rightHistogramBins = rightHistogramGenerator(dataset)

    const rightHistogramYScale = d3
        .scaleLinear()
        .domain(d3.extent(rightHistogramBins, function (d) {
            return d.length
        }))
        .range([dimensions.histogramHeight, 0])

    const rightHistogramBounds = bounds
        .append("g")
        .attr("class", "right-histogram")
        .style("transform", `translate(${dimensions.boundedWidth + dimensions.histogramMargin}px, -${dimensions.histogramHeight}px) rotate(90deg)`)

    const rightHistogramLineGenerator = d3
        .area()
        .x(function (d) {
            return yScale((d.x0 + d.x1) / 2)
        })
        .y0(dimensions.histogramHeight)
        .y1(function (d) {
            return rightHistogramYScale(d.length)
        })
        .curve(d3.curveBasis)

    const rightHistogramElement = rightHistogramBounds
        .append("path")
        .attr("d", function (d) {
            return rightHistogramLineGenerator(rightHistogramBins)
        })
        .attr("class", "histogram-area")

    // 6. Draw peripherals

    const xAxisGenerator = d3
        .axisBottom()
        .scale(xScale)
        .ticks(4)

    const xAxis = bounds
        .append("g")
        .call(xAxisGenerator)
        .style("transform", `translateY(${dimensions.boundedHeight}px)`)

    const xAxisLabel = xAxis
        .append("text")
        .attr("class", "x-axis-label")
        .attr("x", dimensions.boundedWidth / 2)
        .attr("y", dimensions.margin.bottom - 10)
        .html("Minimum Temperature (&deg;F)")

    const yAxisGenerator = d3.axisLeft()
        .scale(yScale)
        .ticks(4)

    const yAxis = bounds.append("g")
        .call(yAxisGenerator)

    const yAxisLabel = yAxis.append("text")
        .attr("class", "y-axis-label")
        .attr("x", -dimensions.boundedHeight / 2)
        .attr("y", -dimensions.margin.left + 10)
        .html("Maximum Temperature (&deg;F)")

    const legendGroup = bounds.append("g")
        .attr("transform", `translate(${dimensions.boundedWidth - dimensions.legendWidth - 9},${dimensions.boundedHeight - 37})`)

    const defs = wrapper.append("defs")

    const numberOfGradientStops = 10
    const stops = d3.range(numberOfGradientStops).map(i => (i / (numberOfGradientStops - 1)))
    const legendGradientId = "legend-gradient"
    const gradient = defs.append("linearGradient")
        .attr("id", legendGradientId)
        .selectAll("stop")
        .data(stops)
        .join("stop")
        .attr("stop-color", d => d3.interpolateRainbow(-d))
        .attr("offset", d => `${d * 100}%`)

    const legendGradient = legendGroup.append("rect")
        .attr("height", dimensions.legendHeight)
        .attr("width", dimensions.legendWidth)
        .style("fill", `url(#${legendGradientId})`)


    const tickValues = [d3.timeParse("%m/%d/%Y")(`4/1/2018`), d3.timeParse("%m/%d/%Y")(`7/1/2018`), d3.timeParse("%m/%d/%Y")(`10/1/2018`),]
    const legendTickScale = d3.scaleLinear()
        .domain(colorScale.domain())
        .range([0, dimensions.legendWidth])

    const legendValues = legendGroup.selectAll(".legend-value")
        .data(tickValues)
        .join("text")
        .attr("class", "legend-value")
        .attr("x", legendTickScale)
        .attr("y", -6)
        .text(d3.timeFormat("%b"))

    const legendValueTicks = legendGroup.selectAll(".legend-tick")
        .data(tickValues)
        .join("line")
        .attr("class", "legend-tick")
        .attr("x1", legendTickScale)
        .attr("x2", legendTickScale)
        .attr("y1", 6)

    // Set up interactions

    // create voronoi for tooltips
    const delaunay = d3.Delaunay
        .from(dataset, d => xScale(xAccessor(d)), d => yScale(yAccessor(d)),)

    const voronoiPolygons = delaunay.voronoi();
    voronoiPolygons.xmax = dimensions.boundedWidth;
    voronoiPolygons.ymax = dimensions.boundedHeight;

    // draw voronoi graph
    const voronoi = dotsGroup.selectAll(".voronoi")
        .data(dataset)
        .join("path")
        .attr("class", "voronoi")
        .attr("d", (d, i) => voronoiPolygons.renderCell(i))

    // add two mouse events in the tooltip

    voronoi.on("mouseenter", onVoronoiMouseEnter)
        .on("mouseleave", onVoronoiMouseLeave)

    const tooltip = d3.select("#tooltip")

    const hoverElementsGroup = bounds.append("g")
        .attr("opacity", 0)

    const dayDot = hoverElementsGroup.append("circle")
        .attr("class", "tooltip-dot")

    // horizontal line at the pointer
    const horizontalLine = hoverElementsGroup.append("rect")
        .attr("class", "hover-line")
    const verticalLine = hoverElementsGroup.append("rect")
        .attr("class", "hover-line")

    function onVoronoiMouseEnter(e, datum) {

        // Given the mouse event and a datum, you are asked to highlight the data by adding a circle and display its information (such as date and temperature).

        const [x, y] = d3.pointer(e);

        const closestDataPointIndex = delaunay.find(x, y);
        const closestDataPoint = dataset[closestDataPointIndex];

        dayDot.attr("cx", xScale(xAccessor(closestDataPoint)))
            .attr("cy", yScale(yAccessor(closestDataPoint)))
            .attr("r", 7)
            .style("stroke", `${colorScale(colorAccessor(closestDataPoint))}`)
            .style("fill", "none");

        // show the halo
        hoverElementsGroup.style("opacity", 1)

        // show the tip
        tooltip.style("opacity", 1)
            .style("transform", `translate(calc(-50% + ${xScale(xAccessor(closestDataPoint)) + dimensions.margin.left}px), ${yScale(yAccessor(closestDataPoint)) + 20}px)`);

        document.getElementById("date").innerHTML = `${dateFormatter(closestDataPoint)}`;
        document.getElementById("min-temperature").innerHTML = `${xAccessor(closestDataPoint)}`;
        document.getElementById("max-temperature").innerHTML = `${yAccessor(closestDataPoint)}`;

        // hover line
        const hoverLineThickness = 10
        // horizontal
        horizontalLine.attr("x", x)
            .attr("y", yScale(yAccessor(closestDataPoint)) - hoverLineThickness / 2)
            .attr("width", dimensions.boundedWidth + dimensions.histogramMargin + dimensions.histogramHeight - x)
            .attr("height", hoverLineThickness)
        // vertical
        verticalLine.attr("x", xScale(xAccessor(closestDataPoint)) - hoverLineThickness / 2)
            .attr("y", -dimensions.histogramMargin - dimensions.histogramHeight)
            .attr("width", hoverLineThickness)
            .attr("height", y + dimensions.histogramMargin + dimensions.histogramHeight)
    }

    function onVoronoiMouseLeave() {
        hoverElementsGroup.style("opacity", 0)
        tooltip.style("opacity", 0)
    }

    // add two mouse actions on the legend
    legendGradient.on("mousemove", onLegendMouseMove)
        .on("mouseleave", onLegendMouseLeave)

    const legendHighlightBarWidth = dimensions.legendWidth * 0.07

    const legendHighlightGroup = legendGroup.append("g")
        .attr("opacity", 0)

    const legendHighlightBar = legendHighlightGroup.append("rect")
        .attr("class", "legend-highlight-bar")
        .attr("width", legendHighlightBarWidth)
        .attr("height", dimensions.legendHeight)

    const legendHighlightText = legendHighlightGroup.append("text")
        .attr("class", "legend-highlight-text")
        .attr("x", legendHighlightBarWidth / 2)
        .attr("y", -6)

    const hoverTopHistogram = topHistogramBounds.append("path")
    const hoverRightHistogram = rightHistogramBounds.append("path")


    function onLegendMouseMove(e) {

        // Display the data only when the data are in the selected date range.

        legendValues.style("opacity", 0)
        legendValueTicks.style("opacity", 0)
        legendHighlightGroup.style("opacity", 1)

        const [x, y] = d3.pointer(e);
        const timestamp2date = function (d) {
            return new Date(d)
        }

        // half month is 1209600000
        const halfMonth = 1209600000
        const startTime = legendTickScale.invert(x) - halfMonth;
        const endTime = legendTickScale.invert(x) + halfMonth;

        // time displayed on the legend
        const leftTime = startTime > 1514782800000 ? dateFormat_legend(timestamp2date(startTime)) : dateFormat_legend(parseDate("2018/01/01"))
        const rightTime = endTime < 1546232400000 ? dateFormat_legend(timestamp2date(endTime)) : dateFormat_legend(parseDate("2018/12/31"))


        // move the highlight bar along your pointer
        legendHighlightBar.style("transform", `translateX(${x - legendHighlightBarWidth / 2}px)`);
        legendHighlightText.style("transform", `translateX(${x - legendHighlightBarWidth / 2}px)`)

        legendHighlightText.text(leftTime + " - " + rightTime);

        const isDayWithinRange = d => {
            // Given a datum, judge whether the datum is in a datum range. Return True or False.

            if (d.time > startTime / 1000 && d.time < endTime / 1000) {
                return true
            }
            return false
        }

        dotsGroup.selectAll(".dot")
            .transition().duration(70)
            .style("opacity", d => isDayWithinRange(d) ? 0.85 : 0.15)
            .attr("r", d => isDayWithinRange(d) ? 4 : 1.5)


        const hoveredDates = dataset.filter(isDayWithinRange);
        const hoveredDate = d3.isoParse(legendTickScale.invert(x))

        // hover top histogram
        hoverTopHistogram.attr("d", d => (topHistogramLineGenerator(topHistogram_generator(hoveredDates))))
            .attr("fill", colorScale(hoveredDate))
            .attr("stroke", "white")
            .style("opacity", 1)


        // hover right histogram
        hoverRightHistogram.attr("d", d => (rightHistogramLineGenerator(rightHistogramGenerator(hoveredDates))))
            .attr("fill", colorScale(hoveredDate))
            .attr("stroke", "white")
            .style("opacity", 1)
    }

    function onLegendMouseLeave() {
        dotsGroup.selectAll(".dot")
            .transition().duration(500)
            .style("opacity", 0.85)
            .attr("r", 3.5)

        legendValues.style("opacity", 1)
        legendValueTicks.style("opacity", 1)
        legendHighlightGroup.style("opacity", 0)

        // hover histogram
        hoverTopHistogram.style("opacity", 0)
        hoverRightHistogram.style("opacity", 0)
    }

}

drawScatter()