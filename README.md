# elizabethsiegle.github.io

A description of this package.

## SF voter guides

`/sf-voter-guides/` hosts the full voter-guide comparison, including ballot filters, guide comparisons, matching, and cited sources. The homepage and voter guide link to the site's public pages.

Rebuild it from the sibling app checkout:

```sh
node scripts/build-sf-voter-guides.mjs ../../demos3idkman/sf-voter-guides
```

The importer validates and builds the app with `/sf-voter-guides/` as its asset base, copies the output into `dist/sf-voter-guides/`, and scopes its security headers to that path. It does not scrape new data or deploy. Data refreshes must be rebuilt into this copy.

Review all pending `dist/` changes before deploying the existing `elizabethsiegle-github-io` Worker with Wrangler.
