// Initialize helper function

let data, stackedBarChart, max, population;

/**
 * Load data from CSV file asynchronously and render area chart
 */
d3.csv('data/A1_Data.csv')
    .then(_data => {
        // get the keys from the data and compute the total population (you are required to use sum function)

        // TODO

        data = _data;

        let keys = Object.keys(data[0]);
        // console.log(keys);

        population = [];
        let popNum =[];

        // state and total population match
        for (const populationElement of data) {
            // console.log(populationElement);

            const {State: _, ...tmp} = populationElement;
            // console.log(tmp);

            let sum = d3.sum(Object.values(tmp));
            let state = populationElement.State;

            population.push({"State": state, "Population": sum});
            popNum.push(sum);
        }

        max = d3.max(popNum);
        data.total = population;

        // Initialize and render chart
        stackedBarChart = new StackedAreaChart(data);
        stackedBarChart.updateVis();
    });


/**
 * Select box event listener
 */

// 
d3.select('#display-type-selection').on('change', function () {
    // Get selected display type and update chart
    stackedBarChart.displayType = d3.select(this).property('value');
    stackedBarChart.updateVis();
});