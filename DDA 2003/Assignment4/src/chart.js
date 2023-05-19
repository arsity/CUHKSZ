async function drawChart() {

    // 1. Access data

    const dataset = await d3.json("./data/education.json")
    // console.log(dataset)
    // get sex variables
    const sexAccessor = d => d.sex
    const sexes = ["female", "male"]
    const sexIds = d3.range(sexes.length)

    // Get education and socioeconomic variables

    const sesAccessor = d => d.ses
    const seses = ["low", "middle", "high"]
    const sesIds = d3.range(seses.length)

    // get a list of different percentages of education levels
    const eduAccessor = d => [d["<High School"] / 100,
        d["High School"] / 100,
        d["Some Post-secondary"] / 100,
        d["Post-secondary"] / 100,
        d["Associate's"] / 100,
        d["Bachelor's and up"] / 100]
    const educationNames = ["<High School",
        "High School",
        "Some Post-secondary",
        "Post-secondary",
        "Associate's",
        "Bachelor's and up"]
    const educationIds = d3.range(educationNames.length)

    // End

    const getStatusKey = ({sex, ses}) => [sex, ses].join("--")
    // Stack Probabilities
    const stackedProbArray = d => {
        let eduList = eduAccessor(d);
        return [
            eduList[0],
            eduList[0] + eduList[1],
            eduList[0] + eduList[1] + eduList[2],
            eduList[0] + eduList[1] + eduList[2] + eduList[3],
            eduList[0] + eduList[1] + eduList[2] + eduList[3] + eduList[4],
            1.0,
        ];
    }
    // getEducation(d): input: d is an object from the dataset
    // return: an integer as following
    // 0 for <High School
    // 1 for High School
    // 2 for Some Post-secondary67890-
    // 3 for Post-secondary
    // 4 for Associate's
    // 5 for Bachelor's and up
    const getEducation = d => d3.bisect(stackedProbArray(d), Math.random())
    // console.log(stackedProbArray(dataset[0]))
    const educationAccessor = d => d.education;
    // End

    // Create person
    let currentPersonId = 0

    function generatePerson(elapsed) {
        const sex = getRandomValue(sexIds)
        const ses = getRandomValue(sesIds)
        const person = dataset.find(p => p.sex === sexes[sex] && p.ses === seses[ses])
        const education = getEducation(person)
        return {
            id: elapsed,
            sex: sex,
            ses: ses,
            education: education,
            startTime: elapsed + getRandomNumberInRange(-0.1, 0.1),
            yJitter: getRandomNumberInRange(-15, 15), // make people to follow the dots easily
        }
    }


    // 2. Create chart dimensions

    const width = d3.min([
        window.innerWidth * 0.9,
        1200
    ])
    let dimensions = {
        width: width,
        height: 500,
        margin: {
            top: 10,
            right: 200,
            bottom: 10,
            left: 120,
        },
        pathHeight: 50,
        endsBarWidth: 15,
        endingBarPadding: 3,
    }
    dimensions.boundedWidth = dimensions.width - dimensions.margin.left - dimensions.margin.right
    dimensions.boundedHeight = dimensions.height - dimensions.margin.top - dimensions.margin.bottom

    // 3. Draw canvas

    const wrapper = d3.select("#wrapper")
        .append("svg")
        .attr("width", dimensions.width)
        .attr("height", dimensions.height)

    const bounds = wrapper.append("g")
        .style("transform", `translate(${dimensions.margin.left}px, ${dimensions.margin.top}px)`)

    // 4. Create scales

    const xScale = d3.scaleLinear()
        .domain([0, 1])
        .range([0, dimensions.boundedWidth - 35])
        .clamp(true)

    const startYScale = d3.scaleLinear()
        .domain([3, -1])
        .range([dimensions.margin.top, dimensions.margin.top + dimensions.boundedHeight])

    const endYScale = d3.scaleLinear()
        .domain([6, -1])
        .range([dimensions.margin.top, dimensions.margin.top + dimensions.boundedHeight])

    const colorScale = d3.scaleLinear()
        .domain([0, 1, 2])
        .range(["rgb(71,183,178)", "rgb(33,88,199)", "rgb(220,36,126)"]);
    // End

    const yTransitionProgressScale = d3.scaleLinear()
        .domain([0.45, 0.55]) // x progress
        .range([0, 1])        // y progress
        .clamp(true)

    // 5. Draw data

    // Define linkGenerator and linkOptions

    const linkOptions = (() => {
        const linkList = []
        for (let ses = 0; ses < 3; ses++) {
            for (let edu = 0; edu < 6; edu++) {
                linkList.push([[ses, edu], [ses, edu], [ses, edu], [ses, edu], [ses, edu], [ses, edu]])
            }
        }
        return linkList
    })()

    const linkLineGenerator = d3.line()
        .x((d, i) => i * (dimensions.boundedWidth / 5))
        .y((d, i) => i < 3 ? startYScale(d[0]) : endYScale(d[1]))
        .curve(d3.curveMonotoneX)

    // End

    const linksGroup = bounds.append("g")
    const links = linksGroup.selectAll(".category-path")
        .data(linkOptions)
        .join("path")
        .attr("class", "category-path")
        .attr("d", linkLineGenerator)
        .attr("stroke-width", dimensions.pathHeight)

    // 6. Draw peripherals

    // start labels
    const startingLabelsGroup = bounds.append("g")
        .style("transform", "translateX(-20px)")

    const startingLabels = startingLabelsGroup.selectAll(".start-label")
        .data(sesIds)
        .join("text")
        .attr("class", "label start-label")
        .attr("y", (d, i) => startYScale(i))
        .text((d, i) => sentenceCase(seses[i]))

    const startLabel = startingLabelsGroup.append("text")
        .attr("class", "start-title")
        .attr("y", startYScale(sesIds[sesIds.length - 1]) - 65)
        .text("Socioeconomic")
    const startLabelLineTwo = startingLabelsGroup.append("text")
        .attr("class", "start-title")
        .attr("y", startYScale(sesIds[sesIds.length - 1]) - 50)
        .text("Status")

    // Add the starting bars on the left
    const startingBars = startingLabelsGroup.selectAll(".start-bar")
        .data(sesIds)
        .join("rect")
        .attr("x", 20)
        .attr("y", d => startYScale(d) - (dimensions.pathHeight / 2))
        .attr("width", dimensions.endsBarWidth)
        .attr("height", dimensions.pathHeight)
        .attr("fill", colorScale)


    // end labels
    const endingLabelsGroup = bounds.append("g")
        .style("transform", `translateX(${
            dimensions.boundedWidth + 20
        }px)`)

    const endingLabels = endingLabelsGroup.selectAll(".end-label")
        .data(educationNames)
        .join("text")
        .attr("class", "label end-label")
        .attr("y", (d, i) => endYScale(i) - 15)
        .text(d => d)

    // drawing circle and triangle for male and female, respectively
    const femaleMarkers = endingLabelsGroup.selectAll(".female-marker")
        .data(educationIds)
        .join("circle")
        .attr("class", "ending-marker female-marker")
        .attr("r", 5.5)
        .attr("cx", 5)
        .attr("cy", d => endYScale(d) + 5)

    const trianglePoints = [
        "-7,  6",
        " 0, -6",
        " 7,  6",
    ].join(" ")
    const maleMarkers = endingLabelsGroup.selectAll(".male-marker")
        .data(educationIds)
        .join("polygon")
        .attr("class", "ending-marker male-marker")
        .attr("points", trianglePoints)
        .attr("transform", d => `translate(5, ${endYScale(d) + 20})`)

    const legendGroup = bounds.append("g")
        .attr("class", "legend")
        .attr("transform", `translate(${dimensions.boundedWidth}, 5)`)

    const MaleLegend = legendGroup.append("g")
        .attr("transform", `translate(${
            -dimensions.endsBarWidth * 1.5
            + dimensions.endingBarPadding
            + 1
        }, 0)`)
    MaleLegend.append("polygon")
        .attr("points", trianglePoints)
        .attr("transform", "translate(-7, 0)")
    MaleLegend.append("text")
        .attr("class", "legend-text-left")
        .text("Male")
        .attr("x", -20)
    MaleLegend.append("line")
        .attr("class", "legend-line")
        .attr("x1", -dimensions.endsBarWidth / 2 + 1)
        .attr("x2", -dimensions.endsBarWidth / 2 + 1)
        .attr("y1", 12)
        .attr("y2", 37)

    const FemaleLegend = legendGroup.append("g")
        .attr("transform", `translate(${
            -dimensions.endsBarWidth / 2
            - 4
        }, 0)`)
    FemaleLegend.append("circle")
        .attr("r", 5.5)
        .attr("transform", "translate(5, 0)")
    FemaleLegend.append("text")
        .attr("class", "legend-text-right")
        .text("Female")
        .attr("x", 15)
    FemaleLegend.append("line")
        .attr("class", "legend-line")
        .attr("x1", dimensions.endsBarWidth / 2 - 3)
        .attr("x2", dimensions.endsBarWidth / 2 - 3)
        .attr("y1", 12)
        .attr("y2", 37)


    // 7. Set up interactions


    // people list  will hold all of simulated people
    // <g> element will hold all people markers
    const maximumPeople = 10000
    let people = []
    const markersGroup = bounds.append("g")
        .attr("class", "markers-group")
    const endingBarGroup = bounds.append("g")
        .attr("transform", `translate(${dimensions.boundedWidth}, 0)`)


    function updateMarkers(elapsed) {
        const xProgressAccessor = d => (elapsed - d.startTime) / 5000 // a person takes 5 seconds (5000 milliseconds) to cross the chart
        if (people.length < maximumPeople) {
            people = [
                ...people,
                ...d3.range(2).map(() => generatePerson(elapsed)), //pass our elapsed milliseconds to each person as they are created
            ]
        }

        // define females and males respectively

        const females = markersGroup.selectAll(".marker-circle")
            .data(people.filter(d => (
                    xProgressAccessor(d) < 1 && sexAccessor(d) === 0)),
                d => d.id)
            .join(
                enter => enter.append("circle")
                    .attr("class", "marker marker-circle")
                    .attr("r", 5.5)
                    .style("opacity", 0)
                ,
                update => update.transition().duration(100)
                    .style("opacity", d => xScale(xProgressAccessor(d)) < 10 ? 0 : 1)
                    .selection()
                // Note that as of v6, we have to call .selection() here
                // This is because without it, we are returning the transition we've created,
                // but selection.join() requires us to return a selection for enter and update groups
                // (but not exit groups)
                ,
                exit => exit.remove()
            )

        const males = markersGroup.selectAll(".marker-triangle")
            .data(people.filter(d => (
                    xProgressAccessor(d) < 1 && sexAccessor(d) === 1)),
                d => d.id)  // key for instructing D3 how to tell our data points apart
            .join(
                enter => enter.append("polygon")
                    .attr("class", "marker marker-triangle")
                    .attr("points", trianglePoints)
                    .style("opacity", 0)
                ,
                update => update.transition().duration(100)
                    .style("opacity", d => xScale(xProgressAccessor(d)) < 10 ? 0 : 1)
                    .selection()
                ,
                exit => exit.remove()
            )
        // End


        const markers = d3.selectAll(".marker")

        markers.style("transform", d => {
            const x = xScale(xProgressAccessor(d))
            const yStart = startYScale(sesAccessor(d))
            const yEnd = endYScale(educationAccessor(d))
            const yChange = yEnd - yStart
            const yProgress = yTransitionProgressScale(
                xProgressAccessor(d)
            )
            const y = yStart
                + (yChange * yProgress)
                + d.yJitter
            return `translate(${x}px, ${y}px)`
        })
            .attr("fill", d => colorScale(sesAccessor(d)))
            .transition().duration(100)
            .style("opacity", d => xScale(xProgressAccessor(d)) < 10
                ? 0
                : 1
            )

        const endingGroups = educationIds.map(endId => (
            people.filter(d => (
                xProgressAccessor(d) >= 1
                && educationAccessor(d) === endId
            ))
        ))
        const endingPercentages = d3.merge(
            endingGroups.map((peopleWithSameEnding, endingId) => (
                d3.merge(
                    sexIds.map(sexId => (
                        sesIds.map(sesId => {
                            const peopleInBar = peopleWithSameEnding.filter(d => (
                                sexAccessor(d) === sexId
                            ))
                            const countInBar = peopleInBar.length
                            const peopleInBarWithSameStart = peopleInBar.filter(d => (
                                sesAccessor(d) === sesId
                            ))
                            const count = peopleInBarWithSameStart.length
                            const numberOfPeopleAbove = peopleInBar.filter(d => (
                                sesAccessor(d) > sesId
                            )).length

                            return {
                                endingId,
                                sesId,
                                sexId,
                                count,
                                countInBar,
                                percentAbove: numberOfPeopleAbove / (peopleInBar.length || 1),
                                percent: count / (countInBar || 1),
                            }
                        })
                    ))
                )
            ))
        )

        endingBarGroup.selectAll(".ending-bar")
            .data(endingPercentages)
            .join("rect")
            .attr("class", "ending-bar")
            .attr("x", d => -dimensions.endsBarWidth * (d.sexId + 1)
                - (d.sexId * dimensions.endingBarPadding)
            )
            .attr("width", dimensions.endsBarWidth)
            .attr("y", d => endYScale(d.endingId)
                - dimensions.pathHeight / 2
                + dimensions.pathHeight * d.percentAbove
            )
            .attr("height", d => d.countInBar
                ? dimensions.pathHeight * d.percent
                : dimensions.pathHeight
            )
            .attr("fill", d => d.countInBar
                ? colorScale(d.sesId)
                : "#dadadd"
            )

        // Update number

        endingLabelsGroup.selectAll(".ending-value")
            .data(endingPercentages)
            .join("text")
            .attr("class", "ending-value")
            .attr("x", d => (d.sesId) * 33 + 47)
            .attr("y", d => endYScale(d.endingId) - dimensions.pathHeight / 2 + 14 * d.sexId + 35)
            .attr("fill", d => d.countInBar ? colorScale(d.sesId) : "#dadadd")  // before activating is gray
            .text(d => d.count)
        // End

    }

    d3.timer(updateMarkers)
}


drawChart()


// utility functions

const getRandomNumberInRange = (min, max) => Math.random() * (max - min) + min

const getRandomValue = arr => arr[Math.floor(getRandomNumberInRange(0, arr.length))]

const sentenceCase = str => [
    str.slice(0, 1).toUpperCase(),
    str.slice(1),
].join("")