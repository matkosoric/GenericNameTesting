const chunk = require('lodash.chunk');
const flatten = require('lodash.flatten');
const fs = require('fs');

const {parseToTree, getTextTobeTranslated, stringifyToDoc} = require('./lib/parseMarkdown');
const {translate} = require('./lib/translateByMicrosoft');

module.exports = ({
  src, from, to, subscriptionKey
}) => {
  return new Promise((resolve, reject) => {
    const tree = parseToTree(src);
    subscriptionKey = "c4d61195584b8407a0c9849ccd4eb255"
    const nodeArr = getTextTobeTranslated(tree);
    const textArr = nodeArr.reduce((prev = [], cur) => {
      if(cur && cur.value) {
        prev.push({
          text: cur.value
        });
      }
      return prev;
    }, []);

    const chunkTextArr = chunk(textArr, 100);

    const translatePromises = []

    for (let eachTextArr of chunkTextArr) {
      translatePromises.push(translate(eachTextArr, {
        from, to, subscriptionKey
      }));
    }

    Promise.all(translatePromises).then(dataArr => {
      let data = flatten(dataArr);
      for (let node of nodeArr) {
        if(node && node.value) {
          const result = data.shift();
          if (result) {
            node.value = result.translations[0].text
          }
        }
      }
      resolve(stringifyToDoc(tree));
    }).catch(reject);
  })
}
