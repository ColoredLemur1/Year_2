#!/bin/bash

# Azure Function Deployment Script
# This script builds, packages, zips, and deploys the Azure Function App

set -e  # Exit on any error

# Configuration (from pom.xml)
FUNCTION_APP_NAME="myfunctionapp14"
RESOURCE_GROUP="java-functions-group"
DEPLOYMENT_DIR="target/azure-functions/${FUNCTION_APP_NAME}"
ZIP_FILE="function-deployment.zip"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Azure Function Deployment Script ===${NC}"
echo ""

# Step 1: Clean and package
echo -e "${YELLOW}Step 1: Building and packaging the function app...${NC}"
mvn clean package -DskipTests

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Maven build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Build completed successfully${NC}"
echo ""

# Step 2: Navigate to deployment directory and create zip
echo -e "${YELLOW}Step 2: Creating deployment ZIP file...${NC}"

if [ ! -d "$DEPLOYMENT_DIR" ]; then
    echo -e "${RED}Error: Deployment directory not found: $DEPLOYMENT_DIR${NC}"
    exit 1
fi

# Change to deployment directory
cd "$DEPLOYMENT_DIR"

# Remove old zip if it exists
if [ -f "../../../$ZIP_FILE" ]; then
    rm "../../../$ZIP_FILE"
    echo "Removed old deployment zip"
fi

# Create zip file (exclude local.settings.json)
zip -r "../../../$ZIP_FILE" host.json DataFunction QueryStatsDB lib sensor-function-app-1.0-SNAPSHOT.jar

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Failed to create ZIP file!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ ZIP file created: $ZIP_FILE${NC}"
echo ""

# Step 3: Return to project root
cd ../../..

# Step 4: Deploy to Azure
echo -e "${YELLOW}Step 3: Deploying to Azure Function App...${NC}"
echo "Function App: $FUNCTION_APP_NAME"
echo "Resource Group: $RESOURCE_GROUP"
echo ""

az functionapp deployment source config-zip \
    --resource-group "$RESOURCE_GROUP" \
    --name "$FUNCTION_APP_NAME" \
    --src "$ZIP_FILE"

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Deployment failed!${NC}"
    exit 1
fi

echo ""
echo -e "${GREEN}✓ Deployment completed successfully!${NC}"
echo ""
echo "You can view logs with:"
echo "  az functionapp log tail --resource-group $RESOURCE_GROUP --name $FUNCTION_APP_NAME"

