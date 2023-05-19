class ForceDirectedGraph {

    /**
     * Class constructor with basic chart configuration
     * @param _config
     * @param _data
     */
    constructor(_config, _data) {
        this.config = {
            parentElement: _config.parentElement,
            containerWidth: 800,
            containerHeight: 800,
            margin: {top: 25, right: 20, bottom: 20, left: 35}
        }
        this.data = _data;
        this.initVis();
    }

    /**
     * We initialize scales/axes and append static elements, such as axis titles.
     */
    initVis() {
        let vis = this;

        // Calculate inner chart size. Margin specifies the space around the actual chart.
        vis.config.width_l = vis.config.containerWidth - vis.config.margin.left - vis.config.margin.right;
        vis.config.height_l = vis.config.containerHeight - vis.config.margin.top - vis.config.margin.bottom;

        vis.colorScale = d3.scaleOrdinal(d3.schemeTableau10);

        // Define size of SVG drawing area
        vis.local = d3.select(vis.config.parentElement).append('svg')
            .attr('width', vis.config.containerWidth)
            .attr('height', vis.config.containerHeight)
            .attr('id', 'local');


        // Append group element that will contain our actual chart
        // and position it according to the given margin config
        vis.chart = vis.local.append("g")
            .attr("transform", `translate(${vis.config.margin.left}, ${vis.config.margin.top})`);


        // define node and link

        // TODO
        vis.nodes = vis.data.nodes
        vis.links = vis.data.links

        // define force simulation

        // TODO
        vis.simulation = d3.forceSimulation(vis.nodes)
            .force("link", d3.forceLink(vis.links).id((d) => d.id))
            .force("charge", d3.forceManyBody())
            .force("center", d3.forceCenter(vis.config.width_l / 2, vis.config.height_l / 2))

        // set force to make sparse
        vis.simulation.force("charge").strength(-80)

        vis.linkedByIndex = {};

        // Build a dictionary (i.e., linkedByIndex) which will be used in isConnected function

        // TODO
        vis.links.forEach((d) => {
            vis.linkedByIndex[`${d.source.index},${d.target.index}`] = 1;
        });

        vis.updateVis();

    }

    /**
     * Prepare the data and scales before we render it.
     */
    updateVis() {
        let vis = this;

        // Add node-link data to simulation

        // TODO
        vis.simulation.nodes(vis.nodes);
        vis.simulation.force("link").links(vis.links);

        // Map color to the node

        // TODO
        vis.colorScale.domain(vis.nodes.map((d) => d.group));

        vis.renderVis();
    }

    /**
     * Bind data to visual elements.
     */
    renderVis() {
        let vis = this;

        // Visualize graph

        // TODO

        // Create links
        let links = vis.local.append("g")
            .attr("class", "links")
            .selectAll("line")
            .data(vis.links)
            .enter().append("line")
            .style("stroke-width", d => Math.sqrt(d.value))
            .attr("opacity", 0.6);

        // Create nodes
        let nodes = vis.local.append("g")
            .attr("class", "nodes")
            .selectAll("circle")
            .data(vis.nodes)
            .enter().append("circle")
            .attr("r", 8)
            .style("fill", d => vis.colorScale(d.group))
            .attr("stroke", "#000")
            .attr("stroke-width", "1px");


        // Set tooltips
        let tooltip = vis.local.append("g")
            .attr("class", "tooltip")
            .selectAll("text")
            .data(vis.nodes)
            .enter().append("text")
            .text(d => d.id)
            .attr("x", d => d.x + 10)
            .attr("y", d => d.y)
            .attr("font-size", "8px")
            .attr("opacity", 0)
            .style("pointer-events", "none")
            .style("text-shadow", "0 0 1px #FFF");

        // define mouse-over event
        nodes.on("mouseover", function (event, d) {
            nodes.transition().duration(200)
                .style("opacity", o => vis.isConnected(d, o) ? 1 : 0.2);
            links.transition().duration(200)
                .style("opacity", o => o.source === d || o.target === d ? 0.6 : 0.2);
            d3.select(event.currentTarget)
                .transition().duration(200)
                .style("opacity", 1);
            tooltip.style("opacity", o => o === d || vis.isConnected(d, o) ? 1 : 0);
        });

        // define mouse-out event
        nodes.on("mouseout", function () {
            nodes.transition().duration(200)
                .style("opacity", 1);
            links.transition().duration(200)
                .style("opacity", 0.6);
            tooltip.style("opacity", 0);
        });


        // Run the simulation
        vis.simulation.nodes(vis.nodes)
            .on("tick", ticked);

        vis.simulation.force("link")
            .links(vis.links);

        function ticked() {
            links
                .attr("x1", d => d.source.x)
                .attr("y1", d => d.source.y)
                .attr("x2", d => d.target.x)
                .attr("y2", d => d.target.y);

            nodes
                .attr("cx", d => d.x)
                .attr("cy", d => d.y);

            tooltip
                .attr("x", d => d.x + 10 + 5)
                .attr("y", d => d.y);
        }


        vis.isConnected = function (a, b) {
            return this.linkedByIndex[`${a.index},${b.index}`] || this.linkedByIndex[`${b.index},${a.index}`];
        };

        this.bonus()
    }

    // this function is all about bonus part
    bonus() {
        const svg = this.local

        // Create a group element for the overview graph rectangle
        const overviewGraphGroup = svg.append("g")
            .attr("id", "overView")

        // Calculate the dimensions and position of the overview graph rectangle
        const overviewWidth = 160;
        const overviewHeight = 160;
        const overviewX = this.config.width_l - overviewWidth - 1;
        const overviewY = this.config.height_l - overviewHeight - 1;

        // Append a rectangle to the overview graph group
        overviewGraphGroup.append("rect")
            .attr("width", overviewWidth)
            .attr("height", overviewHeight)
            .attr("x", overviewX)
            .attr("y", overviewY)
            .attr("fill", "#FFF")
            .attr("stroke", "#8bc1e7");

        // small simulation of overview graph
        const overviewSimulation = d3.forceSimulation(this.nodes)
            .force("charge", d3.forceManyBody())
            .force("center", d3.forceCenter(0, 0));

        // links of overview graph
        const overviewLink = overviewGraphGroup.append("g")
            .attr("class", "links")
            .selectAll("line")
            .data(this.links)
            .enter().append("line")
            .attr("stroke-width", 1)
            .attr("stroke", "#ddd")
            .style("pointer-events", "none")
            .attr("transform", `translate(${overviewX - 20}, ${overviewY - 20})`)

        // nodes of overview graph
        const overviewNode = overviewGraphGroup.append("g")
            .attr("class", "nodes")
            .selectAll("circle")
            .data(this.nodes)
            .enter().append("circle")
            .attr("r", 4)
            .attr("fill", d => this.colorScale(d.group))
            .attr("stroke", "#000")
            .style("pointer-events", "none")
            .attr("transform", `translate(${overviewX - 20}, ${overviewY - 20})`)

        // turn on simulation
        overviewSimulation.on("tick", () => {
            overviewNode.attr("cx", d => d.x / 5 + 100)
                .attr("cy", d => d.y / 5 + 100);

            overviewLink.attr("x1", d => d.source.x / 5 + 100)
                .attr("y1", d => d.source.y / 5 + 100)
                .attr("x2", d => d.target.x / 5 + 100)
                .attr("y2", d => d.target.y / 5 + 100);
        });
    }
}