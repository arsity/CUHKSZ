async function drawMap() {

    // 1. Access data

    const countryShapes = await d3.json("./data/world-geojson.json")
    const dataset = await d3.csv("./data/data_bank_data.csv")

    const countryNameAccessor = d => d.properties["NAME"]
    const countryIdAccessor = d => d.properties["ADM0_A3_IS"]
    const metric = "Population growth (annual %)"

    let metricDataByCountry = {}

    // Process Data

    // TODO

    dataset.forEach(data => {
        if (data["Series Name"] === metric) {
            const metricData = +data["2017 [YR2017]"];
            metricDataByCountry[data["Country Code"]] = isNaN(metricData) ? 0 : metricData;
        }
    })

    // End
    // 2. Create chart dimensions

    let dimensions = {
        width: window.innerWidth * 0.9,
        margin: {
            top: 10,
            right: 10,
            bottom: 10,
            left: 10,
        },
    }
    dimensions.boundedWidth = dimensions.width
        - dimensions.margin.left
        - dimensions.margin.right

    // Projection

    // TODO

    const sphere = ({type: "Sphere"})
    const sphereProjection = d3.geoEqualEarth()
        .fitWidth(dimensions.boundedWidth, sphere)
    const pathGenerator = d3.geoPath(sphereProjection)

    // End


    dimensions.boundedHeight = 800
    dimensions.height = dimensions.boundedHeight
        + dimensions.margin.top
        + dimensions.margin.bottom

    // 3. Draw canvas

    const wrapper = d3.select("#wrapper")
        .append("svg")
        .attr("width", dimensions.width)
        .attr("height", dimensions.height)

    const bounds = wrapper.append("g")
        .style("transform", `translate(${
            dimensions.margin.left
        }px, ${
            dimensions.margin.top
        }px)`)

    // 4. Create scales

    // TODO

    const populationGrowth = Object.values(metricDataByCountry);

    const domain = d3.extent(populationGrowth)
    const maxChange = domain[1]

    const colorScale = d3.scaleLinear()
        .domain([-maxChange, 0, maxChange])
        .range(["indigo", "white", "darkgreen"]);

    // END

    // 5. Draw data

    // TODO
    const earth = bounds.append("path")
        .attr("class", "earth")
        .attr("d", pathGenerator(sphere));

    const graticuleJSON = d3.geoGraticule10();
    const graticule = bounds.append("path")
        .attr("class", "graticule")
        .attr("d", pathGenerator(graticuleJSON));

    const countries = bounds.selectAll(".country")
        .data(countryShapes.features)
        .enter().append("path")
        .attr("class", "country")
        .attr("d", pathGenerator)
        .attr("fill", d => {
            const metricData = metricDataByCountry[countryIdAccessor(d)];
            // console.log("result of id Acc:" ,metricData)
            if (typeof metricData === "undefined") return colorScale(0);
            return colorScale(metricData);
        })

    // END


    // 6. Draw peripherals

    const legendGroup = wrapper.append("g")
        .attr("transform", `translate(${
            120
        },${
            dimensions.width < 800
                ? dimensions.boundedHeight - 30
                : dimensions.boundedHeight * 0.5
        })`)

    const legendTitle = legendGroup.append("text")
        .attr("y", -23)
        .attr("class", "legend-title")
        .text("Population growth")

    const legendByline = legendGroup.append("text")
        .attr("y", -9)
        .attr("class", "legend-byline")
        .text("Percent change in 2017")

    const defs = wrapper.append("defs")
    const legendGradientId = "legend-gradient"
    const gradient = defs.append("linearGradient")
        .attr("id", legendGradientId)
        .selectAll("stop")
        .data(colorScale.range())
        .join("stop")
        .attr("stop-color", d => d)
        .attr("offset", (d, i) => `${
            i * 100 / 2 // 2 is one less than our array's length
        }%`)

    const legendWidth = 120
    const legendHeight = 16
    const legendGradient = legendGroup.append("rect")
        .attr("x", -legendWidth / 2)
        .attr("height", legendHeight)
        .attr("width", legendWidth)
        .style("fill", `url(#${legendGradientId})`)

    const legendValueRight = legendGroup.append("text")
        .attr("class", "legend-value")
        .attr("x", legendWidth / 2 + 10)
        .attr("y", legendHeight / 2)
        .text(`${d3.format(".1f")(maxChange)}%`)

    const legendValueLeft = legendGroup.append("text")
        .attr("class", "legend-value")
        .attr("x", -legendWidth / 2 - 10)
        .attr("y", legendHeight / 2)
        .text(`${d3.format(".1f")(-maxChange)}%`)
        .style("text-anchor", "end")

    navigator.geolocation.getCurrentPosition(myPosition => {
        const [x, y] = sphereProjection([
            myPosition.coords.longitude,
            myPosition.coords.latitude
        ])
        const myLocation = bounds.append("circle")
            .attr("class", "my-location")
            .attr("cx", x)
            .attr("cy", y)
            .attr("r", 0)
            .transition().duration(500)
            .attr("r", 10)
    })

    // 7. Set up interactions

    countries.on("mouseenter", onMouseEnter)
        .on("mouseleave", onMouseLeave)

    const tooltip = d3.select("#tooltip")

    function onMouseEnter(e, datum) {

        // TODO

        tooltip.style("opacity", 0.8)

        const [x, y] = pathGenerator.centroid(datum)

        tooltip.style("transform", `translate(`
            + `calc( -50% + ${x + dimensions.margin.left}px),`
            + `calc( -100% + ${y + dimensions.margin.top}px)`
            + `)`
        )

        const name = countryNameAccessor(datum);
        const metricData = metricDataByCountry[countryIdAccessor(datum)];
        const value = isNaN(metricData) ? 0 : metricData;
        tooltip.html(`<strong>${name}</strong><br>${d3.format(",.2f")(value)}% population change`);

        // END
    }

    function onMouseLeave() {
        tooltip.style("opacity", 0)
    }
}

drawMap()